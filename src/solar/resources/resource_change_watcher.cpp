#include "resource_change_watcher.h"
#include "solar/containers/container_helpers.h"
#include "solar/io/directory_change_watcher.h"
#include "solar/utility/assert.h"
#include "solar/utility/trace.h"
#include "solar/time/real_time.h"

namespace solar {

	resource_change_watcher::watching_resource_info::watching_resource_info() 
		: _handler(nullptr) 
		, _address() {
	}

	resource_change_watcher::watching_resource_info::watching_resource_info(resource_change_handler* handler, const resource_address& address)
		: _handler(handler)
		, _address(address) {
	}

	resource_change_watcher::resource_change_watcher(directory_change_watcher& directory_change_watcher) 
		: _directory_change_watcher(directory_change_watcher) {
	}

	resource_change_watcher::~resource_change_watcher() {
		ASSERT(_watching_resource_infos.empty());
	}

	void resource_change_watcher::setup(const std::vector<resource_provider>& providers) {		
		ASSERT(!_is_setup);
		_is_setup = true;

		ASSERT(_watching_dir_paths.empty());
		for (const auto& provider : providers) {
			if (provider.get_type() == resource_provider_type::FILE_SYSTEM) {
				_watching_dir_paths.push_back(provider.get_root_path());
			}
			else {
				ASSERT(false); //unexpected provider type, ignore?
			}
		}

		//todo - don't support more than one provider to file_system yet (ex. mods). need to change directory
		//change watcher to support multiple directories to watch.
		IF_VERIFY(_watching_dir_paths.size() == 1) {
			_directory_change_watcher.begin_watching_directory(_watching_dir_paths.at(0), true);
		}
	}

	void resource_change_watcher::teardown() {
		if (_is_setup) {
			_directory_change_watcher.end_watching_directory();
			_is_setup = false;
		}
	}

	void resource_change_watcher::begin_watching_resource(resource_change_handler* handler, const resource_address& address) {
		if (_is_setup) {
			ASSERT(!any_if(_watching_resource_infos, [&](const watching_resource_info& info) { return info._handler == handler && info._address == address; }));
			if (address.get_provider_type() == resource_provider_type::FILE_SYSTEM) {
				_watching_resource_infos.emplace_back(handler, address);
			}
			else {
				ASSERT(false); //unexpected provider type, ignore?
			}
		}
	}

	void resource_change_watcher::end_watching_resource(resource_change_handler* handler) {
		remove_and_erase_if(_watching_resource_infos, [&](const watching_resource_info& info) { return info._handler == handler; });
	}

	void resource_change_watcher::try_handle_any_change() {
		if (_is_setup) {
			if (_try_handle_any_change_timer.get_time_elapsed_in_seconds() > 1.f) {
				_try_handle_any_change_timer.reset();

				directory_change dc;
				if (_directory_change_watcher.try_pop_directory_change(dc)) {
					auto iter = find_if(
						_watching_resource_infos,
						[&](const watching_resource_info& info) {
							return info._address.get_file_path() == dc.get_path(); });
					if (iter != _watching_resource_infos.end()) {
						TRACE("resource change detected : {}", iter->_address);
						iter->_handler->on_resource_changed(iter->_address);
					}
				}
			}
		}
	}

}