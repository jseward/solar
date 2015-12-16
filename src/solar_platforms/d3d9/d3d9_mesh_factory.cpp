#include "d3d9_mesh_factory.h"

#include "solar/utility/assert.h"
#include "solar/utility/alert.h"
#include "solar/resources/resource_system.h"
#include "solar/rendering/meshes/mesh_constants.h"
#include "d3d9_context.h"
#include "d3d9_mesh.h"

namespace solar {

	d3d9_mesh_factory::d3d9_mesh_factory(d3d9_context& context, resource_system& resource_system)
		: _context(context)
		, _resource_system(resource_system)
		, _is_setup(false) {
	}

	d3d9_mesh_factory::~d3d9_mesh_factory() {
		ASSERT(!_is_setup);
	}

	void d3d9_mesh_factory::setup() {
		ASSERT(!_is_setup);
		_is_setup = true;
		_context.add_device_event_handler(this);
	}

	void d3d9_mesh_factory::teardown() {
		remove_all_meshs();
		_context.remove_device_event_handler(this);
		_is_setup = false;
	}

	void d3d9_mesh_factory::remove_all_meshs() {
		for (auto& iter : _meshs) {
			iter.second->on_device_lost(_context.get_device());
			iter.second->on_device_released(_context.get_device());
		}
		_meshs.clear();
		_caching_context.increment();
	}

	mesh* d3d9_mesh_factory::get_mesh(const mesh_texture_params& texture_params, const std::string& id, const std::string& id_source_description) {
		ASSERT(_is_setup);

		auto iter = _meshs.find(id);
		if (iter != _meshs.end()) {
			if (iter->second->get_texture_params() != texture_params) {
				ALERT("d3d9_mesh is being requested with different texture_params.\n\nmesh_id: '{}'\nrequested: '{}'\ncached: '{}'", id, texture_params, iter->second->get_texture_params());
			}
			return iter->second.get();
		}

		auto address = _resource_system.resolve_address_to_file("mesh", "meshes", RESOLVE_ADDRESS_MESH_EXTENSIONS, id.c_str(), id_source_description.c_str());
		d3d9_mesh* new_mesh = new d3d9_mesh(*this, texture_params, address);
		_meshs[id] = std::unique_ptr<d3d9_mesh>(new_mesh);

		return new_mesh;
	}

	const resource_factory_caching_context& d3d9_mesh_factory::get_caching_context() const {
		return _caching_context;
	}

	d3d9_context& d3d9_mesh_factory::get_d3d9_context() {
		return _context;
	}

	resource_system& d3d9_mesh_factory::get_resource_system() {
		return _resource_system;
	}

	void d3d9_mesh_factory::on_device_created(IDirect3DDevice9* device) {
		for (auto& iter : _meshs) {
			iter.second->on_device_created(device);
		}
	}

	void d3d9_mesh_factory::on_device_released(IDirect3DDevice9* device) {
		for (auto& iter : _meshs) {
			iter.second->on_device_released(device);
		}
	}

	void d3d9_mesh_factory::on_device_reset(IDirect3DDevice9* device) {
		for (auto& iter : _meshs) {
			iter.second->on_device_reset(device);
		}
	}

	void d3d9_mesh_factory::on_device_lost(IDirect3DDevice9* device) {
		for (auto& iter : _meshs) {
			iter.second->on_device_lost(device);
		}
	}

}
