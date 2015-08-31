#include "win32_file_system.h"
#include <array>
#include "solar/utility/alert.h"
#include "solar/utility/assert.h"
#include "solar/utility/verify.h"
#include "solar/utility/trace.h"
#include "solar/containers/container_helpers.h"
#include "solar/utility/unused_parameter.h"
#include "solar/strings/string_marshal.h"
#include "solar/io/file_path_helpers.h"
#include "solar/strings/string_build.h"
#include "win32_file_stream.h"
#include "win32_headers.h"
#include "win32_error.h"

namespace solar {

	win32_file_system::win32_file_system() {
	}

	win32_file_system::~win32_file_system() {
		ASSERT(_open_file_streams.empty());
	}

	void win32_file_system::setup(const win32_file_system_setup_params& params) {
		setup_user_root_path(
			params.get_user_file_location(),
			params.get_user_root_path_company_name(),
			params.get_user_root_path_app_name());
	}

	file_stream* win32_file_system::open_file_stream(const std::string& path, file_mode mode) {

		const wchar_t* mode_string = L"";
		int share_flag = _SH_DENYWR;

		if (mode == file_mode::OPEN_READ) {
			mode_string = L"rb";
		}
		else if (mode == file_mode::CREATE_WRITE) {
			mode_string = L"wb";
		}
		else if (mode == file_mode::APPEND_WRITE) {
			mode_string = L"ab";
		}
		else {
			ASSERT(false);
		}

		FILE* handle = ::_wfsopen(utf8_to_utf16(path).c_str(), mode_string, share_flag);
		if (handle == NULL) {
			ALERT("Open file failed.\npath : {}\nmode : {}", path, file_mode_details::get_string(mode));
			return nullptr;
		}

		auto fs = new win32_file_stream(path, handle);
		_open_file_streams.push_back(fs);
		return fs;
	}

	void win32_file_system::close_file_stream(file_stream* stream) {
		if (stream != nullptr) {
			win32_file_stream* win32_fs = static_cast<win32_file_stream*>(stream);
			find_and_erase(_open_file_streams, win32_fs);
			VERIFY(::fclose(win32_fs->get_handle()) == 0);
			delete win32_fs;
		}
	}

	void win32_file_system::delete_file(const std::string& path) {
		if (::DeleteFile(utf8_to_utf16(path).c_str()) == 0) {
			ALERT("Delete file failed.\npath : '{}'\nerror : {}", path, get_last_win32_error_message());
		}
	}

	bool win32_file_system::copy_file(const std::string& src_path, const std::string& dst_path) {
		if (::CopyFile(utf8_to_utf16(src_path).c_str(), utf8_to_utf16(dst_path).c_str(), FALSE) == FALSE) {
			ALERT("Copy file failed.\nsrc_path : '{}'\ndst_path : '{}'\nerror : {}", src_path, dst_path, get_last_win32_error_message());
			return false;
		}
		return true;
	}

	void win32_file_system::set_is_file_read_only(const std::string& path, bool is_read_only) {
		const DWORD attributes = is_read_only ?
			FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_ARCHIVE :
			FILE_ATTRIBUTE_NORMAL;
		if (::SetFileAttributes(utf8_to_utf16(path).c_str(), attributes) != TRUE) {
			ALERT("set_is_file_read_only failed.\npath : '{}'\nerror : {}", path, get_last_win32_error_message());
		}
	}

	bool win32_file_system::does_file_exist(const std::string& file_path) const {
		DWORD attributes = ::GetFileAttributes(utf8_to_utf16(file_path).c_str());
		return
			(attributes != INVALID_FILE_ATTRIBUTES) &&
			((attributes & FILE_ATTRIBUTE_DIRECTORY) == 0);
	}

	bool win32_file_system::does_directory_exist(const std::string& dir_path) const {
		//http://stackoverflow.com/questions/6218325/how-do-you-check-if-a-directory-exists-on-windows-in-c
		DWORD attributes = ::GetFileAttributes(utf8_to_utf16(dir_path).c_str());
		return
			(attributes != INVALID_FILE_ATTRIBUTES) &&
			((attributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
	}

	void win32_file_system::create_directory(const std::string& dir_path) {
		TRACE("file_system : create_directory : {}", dir_path);
		if (::CreateDirectory(utf8_to_utf16(dir_path).c_str(), nullptr) == 0) {
			ALERT("Create directory failed.\npath : {}\nerror : {}", dir_path, get_last_win32_error_message());
		}
	}

	void win32_file_system::setup_user_root_path(win32_user_file_location location, const std::string& company_folder_name, const std::string& app_folder_name) {
		ASSERT(_user_root_path.empty());

		if (location == win32_user_file_location::USER_APPDATA) {
			ASSERT(!company_folder_name.empty());
			ASSERT(!app_folder_name.empty());

			wchar_t user_root_path_buffer[_MAX_PATH] = L"";
			VERIFY(SUCCEEDED(::SHGetFolderPathW(NULL, CSIDL_LOCAL_APPDATA | CSIDL_FLAG_CREATE, NULL, 0, user_root_path_buffer))); //Can't use ::SHGetFolderPathAndSubDir since it doesn't work on Win2K.
			VERIFY(::PathAppend(user_root_path_buffer, utf8_to_utf16(company_folder_name).c_str()) == TRUE);
			VERIFY(::PathAppend(user_root_path_buffer, utf8_to_utf16(app_folder_name).c_str()) == TRUE);

			_user_root_path = utf16_to_utf8(user_root_path_buffer);
		}
		else if (location == win32_user_file_location::WORKING_DIR) {
			ASSERT(company_folder_name.empty());
			ASSERT(app_folder_name.empty());

			_user_root_path = resolve_file_path(".");
		}

		//Create the directories needed for files to be created in this path.
		create_directories_if_needed(_user_root_path);
		TRACE("file_system user_root_path set to '{}'", _user_root_path);
	}

	std::string win32_file_system::make_user_file_dir_path(const std::string& folder) {
		ASSERT(!_user_root_path.empty()); //setup_user_root_path() not called?
		auto dir = make_file_path(_user_root_path, folder);
		create_directories_if_needed(dir);
		return dir;
	}

	std::string win32_file_system::make_user_file_path(const std::string& folder, const std::string& file_name) {
		auto dir = make_user_file_dir_path(folder);;
		return make_file_path(dir, file_name);
	}

	void win32_file_system::create_directories_if_needed(const std::string& dir_path) {
		if (!does_directory_exist(dir_path)) {

			//First recursively create all parent directories if needed.
			auto pos = dir_path.find_last_of(FILE_PATH_SEPERATOR_CHAR);
			if (pos != std::string::npos) {
				auto parent_dir_path = dir_path.substr(0, pos);
				create_directories_if_needed(parent_dir_path);
			}

			//Check again that directory still doesn't exist. this handles case where "a\b\" and "a\b" (same dir) both try to be created gracefully.
			if (!does_directory_exist(dir_path)) {
				TRACE("file_system : creating directory that does not exist : '{}'", dir_path);
				create_directory(dir_path);
			}
		}
	}

	std::string win32_file_system::resolve_file_path(const std::string relative_path) {
		auto relative_path_utf16 = utf8_to_utf16(relative_path);
		std::array<wchar_t, _MAX_PATH> full_path_utf16;
		VERIFY(::_wfullpath(full_path_utf16.data(), relative_path_utf16.c_str(), full_path_utf16.size()) != nullptr);
		return utf16_to_utf8(full_path_utf16.data());
	}

	std::vector<std::string> win32_file_system::find_file_names(const std::string& dir_path, const find_file_names_options& options) {
		std::vector<std::string> file_names;

		//if we don't auto-append \* then FindFirstFileW will simply return a handle to the directory itself and find nothing.
		auto find_path = build_string("{}\\*", dir_path);

		WIN32_FIND_DATAW find_data;
		HANDLE handle = ::FindFirstFileW(utf8_to_utf16(find_path).c_str(), &find_data);
		while (handle != INVALID_HANDLE_VALUE) {

			if (!is_find_data_hidden(find_data)) {
				if (is_find_data_directory(find_data)) {
					if (options._is_recursive) {
						auto sub_dir_path = make_file_path(dir_path, utf16_to_utf8(find_data.cFileName));
						auto sub_dir_file_names = find_file_names(sub_dir_path, options);
						file_names.insert(file_names.end(), sub_dir_file_names.begin(), sub_dir_file_names.end());
					}
				}
				else {
					auto file_name = utf16_to_utf8(find_data.cFileName);
					if (check_file_name_with_find_file_names_options(file_name, options)) {
						file_names.push_back(make_file_path(dir_path, file_name));
					}
				}
			}

			if (::FindNextFileW(handle, &find_data) == FALSE) {
				handle = INVALID_HANDLE_VALUE;
			}
		}

		return file_names;
	}

	bool win32_file_system::is_directory(const std::string& path) {
		//https://msdn.microsoft.com/en-us/library/bb773621(VS.85).aspx
		return (::PathIsDirectory(utf8_to_utf16(path).c_str()) == FILE_ATTRIBUTE_DIRECTORY);
	}

	bool win32_file_system::try_get_last_write_time(date_time& last_write_time, const std::string& path) {
		WIN32_FIND_DATAW fd = { 0 };
		HANDLE h = ::FindFirstFileW(utf8_to_utf16(path).c_str(), &fd);
		if (h != INVALID_HANDLE_VALUE) {
			::FindClose(h);
			last_write_time = convert_FILETIME_to_date_time(fd.ftLastWriteTime);
			return true;
		}
		return false;
	}

	date_time win32_file_system::convert_FILETIME_to_date_time(const FILETIME& ft) const {
		//http://www.gamedev.net/topic/565693-converting-filetime-to-time_t-on-windows/
		ULARGE_INTEGER ull;
		ull.LowPart = ft.dwLowDateTime;
		ull.HighPart = ft.dwHighDateTime;
		return make_date_time_from_time_t((ull.QuadPart / 10000000ULL) - 11644473600ULL);
	}

	bool win32_file_system::check_file_name_with_find_file_names_options(const std::string& file_name, const find_file_names_options& options) const {
		if (options._filtered_extensions.empty()) {
			return true;
		}

		auto ext = get_file_extension(file_name);
		for (const auto& filtered_ext : options._filtered_extensions) {
			if (ext == filtered_ext) {
				return true;
			}
		}

		return false;
	}

	bool win32_file_system::is_find_data_hidden(const WIN32_FIND_DATAW& find_data) const {
		return 
			(wcscmp(find_data.cFileName, L".") == 0) ||
			(wcscmp(find_data.cFileName, L"..") == 0);
	}

	bool win32_file_system::is_find_data_directory(const WIN32_FIND_DATAW& find_data) const {
		return ((find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0UL);
	}

}
