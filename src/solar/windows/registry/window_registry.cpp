#include "window_registry.h"

#include "solar/utility/assert.h"
#include "solar/utility/unused_parameter.h"
#include "solar/containers/container_helpers.h"
#include "solar/windows/window.h"
#include "solar/resources/resource_system.h"
#include "window_registry_window_collection.h"

namespace solar {

	window_registry::window_registry(resource_system& resource_system) 
		: _resource_system(resource_system) 
		, _has_loaded_all(false) {
	}

	window_registry::~window_registry() {
		ASSERT(_window_collections.empty());
	}

	void window_registry::add_window(window* window) {
		ASSERT(!_has_loaded_all);
		auto iter = find_window_collection(window);
		if (iter != _window_collections.end()) {
			(*iter)->add_window(window);
		}
		else {
			_window_collections.push_back(std::make_unique<window_registry_window_collection>(window));
		}
	}

	void window_registry::remove_window(window* window) {
		auto iter = find_window_collection(window);
		IF_VERIFY(iter != _window_collections.end()) {
			(*iter)->remove_window(window);
			if ((*iter)->empty()) {
				_resource_system.end_watching_resource(this, iter->get());
				_window_collections.erase(iter);
			}
		}
	}

	window_registry::window_collection_vector::iterator window_registry::find_window_collection(window* window) {
		return find_if(_window_collections, [&window](const window_collection_vector::value_type& collection) {
			return collection->get_id() == window->get_id();
		});
	}

	void window_registry::load_all() {
		for (auto& iter : _window_collections) {
			auto address = resolve_window_collection_address(*iter);
			iter->load(_resource_system, address);
			_resource_system.begin_watching_resource(this, address, iter.get());
		}
		_has_loaded_all = true;
	}

	void window_registry::teardown() {
		_resource_system.end_watching_resources(this);
	}

	resource_address window_registry::resolve_window_collection_address(const window_registry_window_collection& collection) {
		return _resource_system.resolve_address_to_file("window", "windows", ".window", collection.get_id().c_str(), "");
	}

	void window_registry::on_file_changed(const std::string& path, void* data) {
		UNUSED_PARAMETER(path);
		window_registry_window_collection* collection = reinterpret_cast<window_registry_window_collection*>(data);
		collection->load(_resource_system, resolve_window_collection_address(*collection));
	}

}
