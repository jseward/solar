#include "d3d9_cursor_icon_factory.h"

#include "solar/resources/resource_system.h"
#include "solar/resources/resource_mapped_memory.h"
#include "solar/resources/resource_id_manifest.h"
#include "solar/utility/assert.h"
#include "solar/utility/alert.h"
#include "d3d9_cursor.h"
#include "d3d9_cursor_icon.h"
#include "d3d9_context.h"

namespace solar {

	d3d9_cursor_icon_factory::d3d9_cursor_icon_factory(d3d9_context& context, d3d9_cursor& cursor, resource_system& resource_system)
		: _context(context)
		, _cursor(cursor)
		, _resource_system(resource_system) {

		_resource_mapped_memory = std::make_unique<resource_mapped_memory>();
	}

	d3d9_cursor_icon_factory::~d3d9_cursor_icon_factory() {
		ASSERT(_icons.empty());
	}

	void d3d9_cursor_icon_factory::setup() {
		ASSERT(_icons.empty());
		
		static const char* FOLDER_NAME = "cursors";

		resource_id_manifest manifest;
		manifest.load(_resource_system, "cursor_icon_manifest", FOLDER_NAME, ".cursor_icon_manifest", "_cursor_icon_manifest", "d3d9_cursor_icon_factory");
		for (auto id : manifest.get_ids()) {
			auto address = _resource_system.resolve_address("cursor_icon", FOLDER_NAME, ".cursor_icon", id.c_str(), manifest.get_address().to_string().c_str());
			_icons[id] = std::make_unique<d3d9_cursor_icon>(*this, address);
			_icons[id]->load_def();
		}

		_context.add_device_event_handler(this);
		_caching_context.increment();
	}

	void d3d9_cursor_icon_factory::teardown() {
		_context.remove_device_event_handler(this);
		_cursor.remove_icon();
		_icons.clear();
		_empty_icon.release();
		_caching_context.increment();
	}

	cursor_icon* d3d9_cursor_icon_factory::get_cursor_icon(const std::string& id, const std::string& id_source_description) {
		auto iter = _icons.find(id);
		if (iter != _icons.end()) {
			return iter->second.get();
		}

		ALERT("cursor_icon not found.\n\nid : {0}\nid_source_description : {1}", id, id_source_description);
		if (_empty_icon == nullptr) {
			_empty_icon = std::make_unique<d3d9_cursor_icon>(*this, resource_address());
		}
		return _empty_icon.get();

	}

	const resource_factory_caching_context& d3d9_cursor_icon_factory::get_caching_context() const {
		return _caching_context;
	}

	d3d9_cursor& d3d9_cursor_icon_factory::get_cursor() {
		return _cursor;
	}

	resource_system& d3d9_cursor_icon_factory::get_resource_system() {
		return _resource_system;
	}

	resource_mapped_memory& d3d9_cursor_icon_factory::get_resource_mapped_memory() {
		return *_resource_mapped_memory.get();
	}

	void d3d9_cursor_icon_factory::on_device_created(IDirect3DDevice9* device) {
		for (auto& iter : _icons) {
			iter.second->on_device_created(device);
		}
	}

	void d3d9_cursor_icon_factory::on_device_released(IDirect3DDevice9* device) {
		for (auto& iter : _icons) {
			iter.second->on_device_released(device);
		}
	}

	void d3d9_cursor_icon_factory::on_device_reset(IDirect3DDevice9* device) {
		for (auto& iter : _icons) {
			iter.second->on_device_reset(device);
		}
	}

	void d3d9_cursor_icon_factory::on_device_lost(IDirect3DDevice9* device) {
		for (auto& iter : _icons) {
			iter.second->on_device_lost(device);
		}
	}


}