#include "win32_directory_change_watcher.h"
#include "win32_headers.h"
#include "win32_error.h"
#include "solar/strings/string_marshal.h"
#include "solar/strings/string_convert.h"
#include "solar/utility/assert.h"
#include "solar/utility/alert.h"
#include "solar/utility/trace.h"
#include "solar/utility/verify.h"
#include "solar/io/file_path_helpers.h"
#include "solar/time/real_time.h"
#include "solar/threading/thread_helpers.h"

//useful references:
//http://qualapps.blogspot.ca/2010/05/understanding-readdirectorychangesw_19.html

namespace solar {

	win32_directory_change_watcher::win32_directory_change_watcher() {
	}

	win32_directory_change_watcher::~win32_directory_change_watcher() {
		ASSERT(_reading_thread == nullptr);
	}

	void win32_directory_change_watcher::begin_watching_directory(const std::string& dir_path, bool is_recursive) {

		TRACE("watching directory for changes '{}' (is_recursive:{})", dir_path, bool_to_string(is_recursive));

		_dir_path = dir_path;

		_h_dir = ::CreateFile(
			utf8_to_utf16(dir_path).c_str(),
			FILE_LIST_DIRECTORY,
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
			NULL,
			OPEN_EXISTING,
			FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
			NULL);
		if (_h_dir.get() == INVALID_HANDLE_VALUE) {
			ALERT("win32_directory_change_watcher creating handle to directory failed.\ndir_path : '{}'\nerror : {}", dir_path, get_last_win32_error_message());
		}
		else {

			_h_iocp = ::CreateIoCompletionPort(_h_dir.get(), NULL, 0, 1);
			if (_h_iocp.get() == NULL) {
				ALERT("win32_directory_change_watcher creating IoCompletionPort failed.\nerror : {}", get_last_win32_error_message());
			}
			else {

				ASSERT(_reading_thread == nullptr);
				_reading_thread = std::unique_ptr<std::thread>(new std::thread(&win32_directory_change_watcher::run_in_thread, this, _h_iocp.get(), _h_dir.get(), is_recursive));

			}
		}
	}

	void win32_directory_change_watcher::end_watching_directory() {
		if (_reading_thread != nullptr) {
			
			//signal the iocp handler in thread to exit.
			VERIFY(::PostQueuedCompletionStatus(_h_iocp.get(), 0, 0, NULL) != 0);

			_reading_thread->join();
			_reading_thread.release();
		}

		_h_dir.close();
		_h_iocp.close();

		_staged_changes.clear();
		_changes = {};
	}

	bool win32_directory_change_watcher::try_pop_directory_change(directory_change& change) {
		
		try_complete_staged_changes();

		if (!_changes.empty()) {
			change = std::move(_changes.front());
			_changes.pop();
			return true;
		}
		return false;
	}

	void win32_directory_change_watcher::try_complete_staged_changes() {
		
		const float STAGED_CHANGE_DURATION = 1.f;

		if (!_staged_changes.empty()) {
			std::lock_guard<std::mutex> lock(_staged_changes_mutex);
			auto iter = _staged_changes.begin();
			while (iter != _staged_changes.end()) {
				if (get_real_time_in_seconds() - iter->second._last_change_time > STAGED_CHANGE_DURATION) {

					auto full_path = make_file_path(_dir_path, iter->first);

					directory_change_type change_type = directory_change_type::invalid;
					switch (iter->second._fni_action) {
					case FILE_ACTION_ADDED:
						change_type = directory_change_type::ITEM_ADDED;
						break;
					case FILE_ACTION_REMOVED:
						change_type = directory_change_type::ITEM_REMOVED;
						break;
					case FILE_ACTION_MODIFIED:
						change_type = directory_change_type::ITEM_MODIFIED;
						break;
					case FILE_ACTION_RENAMED_OLD_NAME:
						//ignoring this action
						break;
					case FILE_ACTION_RENAMED_NEW_NAME:
						change_type = directory_change_type::ITEM_RENAMED;
						break;
					}

					if (change_type != directory_change_type::invalid) {
						_changes.emplace(change_type, full_path);
					}

					iter = _staged_changes.erase(iter);
				}
				else {
					iter++;
				}
			}
		}
	}

	void win32_directory_change_watcher::run_in_thread(HANDLE h_iocp, HANDLE h_dir, bool is_recursive) {

		set_current_thread_name("win32_directory_change_watcher");

		for (;;) {

			const DWORD notify_filter =
				FILE_NOTIFY_CHANGE_LAST_WRITE |
				FILE_NOTIFY_CHANGE_FILE_NAME |
				FILE_NOTIFY_CHANGE_DIR_NAME;

			auto buffer = std::make_unique<std::array<char, 1024>>();
			OVERLAPPED overlapped = { 0 };

			BOOL read_result = ::ReadDirectoryChangesW(
				h_dir,
				buffer->data(),
				buffer->size(),
				is_recursive,
				notify_filter,
				NULL,
				&overlapped,
				NULL);
			if (read_result == 0) {
				ALERT("win32_directory_change_watcher ReadDirectoryChangesW failed.\nerror : {}", get_last_win32_error_message());
				break;
			}

			DWORD bytes_transfered = 0;
			ULONG_PTR completion_key = 0;
			OVERLAPPED* overlapped_out = NULL;

			BOOL get_status_result = ::GetQueuedCompletionStatus(h_iocp, &bytes_transfered, &completion_key, &overlapped_out, INFINITE);
			if (get_status_result == 0) {
				ALERT("win32_directory_change_watcher GetQueuedCompletionStatus failed.\nerror : {}", get_last_win32_error_message());
				break;
			}
			else {
				if (overlapped_out == NULL) {
					//end_watching_directory() will force this to happen.
					break;
				}
				else {
					IF_VERIFY(bytes_transfered > 0) {
						handle_notifications_in_thread(buffer.get()->data(), bytes_transfered);
					}
				}
			}

		}

	}

	void win32_directory_change_watcher::handle_notifications_in_thread(const char* buffer, DWORD size) {
		const FILE_NOTIFY_INFORMATION* fni = reinterpret_cast<const FILE_NOTIFY_INFORMATION*>(buffer);
		handle_notification_in_thread(fni);
		while (fni->NextEntryOffset != 0) {
			fni = reinterpret_cast<const FILE_NOTIFY_INFORMATION*>(reinterpret_cast<const char*>(fni) + fni->NextEntryOffset);
			ASSERT(static_cast<DWORD>((reinterpret_cast<const char*>(fni) - buffer)) < size);
			handle_notification_in_thread(fni);
		}
	}

	void win32_directory_change_watcher::handle_notification_in_thread(const FILE_NOTIFY_INFORMATION* fni) {
		
		wchar_t w_file_name[_MAX_PATH] = { 0 };
		size_t file_name_length_in_chars = fni->FileNameLength / sizeof(wchar_t); //fni->FileNameLength is in bytes
		::wmemcpy_s(w_file_name, _MAX_PATH - 1, fni->FileName, file_name_length_in_chars); //_MAX_PATH - 1 to guarantee string ends with \0
		auto file_name = utf16_to_utf8(w_file_name);

		if (fni->Action != FILE_ACTION_RENAMED_OLD_NAME) {
			std::lock_guard<std::mutex> lock(_staged_changes_mutex);
			auto iter = _staged_changes.find(file_name);
			if (iter == _staged_changes.end()) {
				_staged_changes[file_name] = staged_change(fni->Action, get_real_time_in_seconds());
			}
			else {
				//this is a bit tricky as multiple changes to the same file are happening quickly. exporting an .fbx file overtop an
				//existing .fbx file will cause 3 events REMOVE,ADD,MODIFIED in quick succession. I used to just ignore the action but
				//REMOVE is lower priority than the others (ex. peon tool ignores REMOVE). so instead just overwrite action to most recent
				//action so MODIFIED would be the remaining action.
				iter->second._fni_action = fni->Action;
				iter->second._last_change_time = get_real_time_in_seconds();
			}
		}
	}

}