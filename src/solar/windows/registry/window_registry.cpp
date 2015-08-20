#include "window_registry.h"
#include "solar/utility/assert.h"
#include "solar/containers/container_helpers.h"
#include "solar/windows/window.h"
#include "window_registry_window_collection.h"

namespace solar {

	window_registry::window_registry(resource_system& resource_system) 
		: _resource_system(resource_system) {
	}

	window_registry::~window_registry() {
		ASSERT(_window_collections.empty());
	}

	void window_registry::add_window(window* window) {
		auto iter = find_window_collection(window);
		if (iter != _window_collections.end()) {
			(*iter)->add_window(window);
		}
		else {
			_window_collections.push_back(std::make_unique<window_registry_window_collection>(_resource_system, window));
		}
	}

	void window_registry::remove_window(window* window) {
		auto iter = find_window_collection(window);
		IF_VERIFY(iter != _window_collections.end()) {
			(*iter)->remove_window(window);
			if ((*iter)->empty()) {
				_window_collections.erase(iter);
			}
		}
	}

	window_registry::window_collection_vector::iterator window_registry::find_window_collection(window* window) {
		return find_if(_window_collections, [&window](const window_collection_vector::value_type& collection) {
			return collection->get_id() == window->get_id();
		});
	}

	void window_registry::load() {
		for (auto& iter : _window_collections) {
			iter->load();
		}
	}

	void window_registry::save() {
		for (auto& iter : _window_collections) {
			iter->save();
		}
	}

}
