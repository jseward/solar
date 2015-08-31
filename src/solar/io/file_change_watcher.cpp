#include "file_change_watcher.h"

#include "solar/containers/container_helpers.h"
#include "solar/io/directory_change_watcher.h"
#include "solar/utility/alert.h"
#include "solar/utility/assert.h"
#include "solar/utility/trace.h"
#include "solar/time/real_time.h"
#include "solar/strings/string_helpers.h"

namespace solar {

	file_change_watcher::watched_file_info::watched_file_info()
		: _handler(nullptr) 
		, _path() {
	}

	file_change_watcher::watched_file_info::watched_file_info(file_change_handler* handler, const std::string& path)
		: _handler(handler)
		, _path(path) {
	}

	file_change_watcher::file_change_watcher(directory_change_watcher& directory_change_watcher)
		: _directory_change_watcher(directory_change_watcher) {
	}

	file_change_watcher::~file_change_watcher() {
		ASSERT(_watched_file_infos.empty());
	}

	void file_change_watcher::setup(const std::vector<std::string>& dir_paths) {		
		ASSERT(!_is_setup);
		_is_setup = true;

		ASSERT(_watched_dir_paths.empty());
		_watched_dir_paths = dir_paths;

		for (const auto& dir_path : dir_paths) {
			_directory_change_watcher.begin_watching_directory(dir_path, true);
		}
	}

	void file_change_watcher::teardown() {
		if (_is_setup) {
			for (const auto& dir_path : _watched_dir_paths) {
				_directory_change_watcher.end_watching_directory(dir_path);
			}
			_watched_dir_paths.clear();
			_is_setup = false;
		}
	}

	void file_change_watcher::begin_watching_file(file_change_handler* handler, const std::string& path) {
		if (_is_setup) {
			ASSERT(!any_if(_watched_file_infos, [&](const watched_file_info& info) { return info._handler == handler && info._path == path; })); //double registered?
			if (!any_if(_watched_dir_paths, [&](const std::string& dir_path) { return does_string_start_with(path.c_str(), dir_path.c_str()); })) {
				ALERT("Attempt to watch file located in an unwatched directory.\n\npath : '{}'", path);
			}
			_watched_file_infos.emplace_back(handler, path);
		}
	}

	void file_change_watcher::end_watching_file(file_change_handler* handler) {
		remove_and_erase_if(_watched_file_infos, [&](const watched_file_info& info) { return info._handler == handler; });
	}

	void file_change_watcher::try_handle_any_change() {
		if (_is_setup) {
			if (_try_handle_any_change_timer.get_time_elapsed_in_seconds() > 1.f) {
				_try_handle_any_change_timer.reset();

				directory_change dc;
				if (_directory_change_watcher.try_pop_directory_change(dc)) {
					auto iter = find_if(_watched_file_infos, [&](const watched_file_info& info) { return info._path == dc.get_path(); });
					if (iter != _watched_file_infos.end()) {
						TRACE("file change detected : {}", iter->_path);
						iter->_handler->on_file_changed(iter->_path);
					}
				}
			}
		}
	}

}