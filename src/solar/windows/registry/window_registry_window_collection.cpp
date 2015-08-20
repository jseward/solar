#include "window_registry_window_collection.h"
#include "solar/containers/container_helpers.h"
#include "solar/resources/resource_system.h"
#include "solar/archiving/json_archive_reader.h"
#include "solar/archiving/archivable_cloning.h"
#include "solar/windows/window.h"

namespace solar {

	window_registry_window_collection::window_registry_window_collection(resource_system& resource_system, window* w)
		: _resource_system(resource_system)
		, _id(w->get_id()) {

		_windows.push_back(w);
	}

	window_registry_window_collection::~window_registry_window_collection() {
	}

	const std::string& window_registry_window_collection::get_id() const {
		return _id;
	}

	bool window_registry_window_collection::empty() const {
		return _windows.empty();
	}

	void window_registry_window_collection::add_window(window* window) {
		push_back_and_verify_not_found(_windows, window);
	}

	void window_registry_window_collection::remove_window(window* window) {
		find_and_erase(_windows, window);
	}

	void window_registry_window_collection::load() {
		ASSERT(!_windows.empty());

		auto address = _resource_system.resolve_address("window", "windows", ".window", _id.c_str(), "");
		if (!address.empty()) {
			auto first_window = *_windows.begin();
			if (_resource_system.read_object_as_json(*first_window, address)) {
				handle_loaded_recursive(*first_window);

				//clone other windows from the first window
				for (auto w : _windows) {
					if (w != first_window) {
						clone_archivable_object(*w, *first_window);
						handle_loaded_recursive(*w);
					}
				}

			}
		}
	}

	void window_registry_window_collection::handle_loaded_recursive(window& window) {
		window.on_loaded();
		for (auto& child : window.get_children()) {
			handle_loaded_recursive(*child);
		}
	}

	void window_registry_window_collection::save() {
		auto address = _resource_system.resolve_address("window", "windows", ".window", _id.c_str(), "");
		if (!address.empty()) {
			_resource_system.write_object_as_json(**_windows.begin(), address);
		}
	}

}
