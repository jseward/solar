#include "win32_directory_change_watcher.h"

#include "solar/utility/assert.h"
#include "solar/utility/verify.h"
#include "solar/containers/container_helpers.h"

namespace solar {

	win32_directory_change_watcher::win32_directory_change_watcher() {
	}

	win32_directory_change_watcher::~win32_directory_change_watcher() {
		ASSERT(_watchers.empty());
	}

	void win32_directory_change_watcher::begin_watching_directory(const std::string& dir_path, bool is_recursive) {
		auto lambda = [&](const watchers_type::value_type& watcher) { return watcher->get_dir_path() == dir_path; };
		ASSERT(!any_if(_watchers, lambda));

		auto new_watcher = std::make_unique<win32_single_directory_change_watcher>(dir_path, is_recursive);
		new_watcher->begin_watching();
		_watchers.push_back(std::move(new_watcher));
	}

	void win32_directory_change_watcher::end_watching_directory(const std::string& dir_path) {
		auto iter = find_if(_watchers, [&](const watchers_type::value_type& watcher) { return watcher->get_dir_path() == dir_path; });
		IF_VERIFY(iter != _watchers.end()) {
			(*iter)->end_watching();
			_watchers.erase(iter);
		}
	}

	bool win32_directory_change_watcher::try_pop_directory_change(directory_change& change) {
		for (auto& watcher : _watchers) {
			if (watcher->try_pop_directory_change(change)) {
				return true;
			}
		}
		return false;
	}

}