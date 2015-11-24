#include "bgfx_mesh_factory.h"

#include "solar/utility/assert.h"
#include "solar/utility/alert.h"
#include "solar/resources/resource_system.h"
#include "solar/rendering/meshes/mesh_constants.h"
#include "bgfx_mesh.h"

namespace solar {

	bgfx_mesh_factory::bgfx_mesh_factory(resource_system& resource_system)
		: _resource_system(resource_system)
		, _is_setup(false) {
	}

	bgfx_mesh_factory::~bgfx_mesh_factory() {
		ASSERT(!_is_setup);
	}

	void bgfx_mesh_factory::setup() {
		ASSERT(!_is_setup);
		_is_setup = true;
	}

	void bgfx_mesh_factory::teardown() {
		remove_all_meshs();
		_is_setup = false;
	}

	void bgfx_mesh_factory::remove_all_meshs() {
		_meshs.clear();
		_caching_context.increment();
	}

	mesh* bgfx_mesh_factory::get_mesh(const mesh_texture_params& texture_params, const std::string& id, const std::string& id_source_description) {
		ASSERT(_is_setup);

		auto iter = _meshs.find(id);
		if (iter != _meshs.end()) {
			if (iter->second->get_texture_params() != texture_params) {
				ALERT("bgfx_mesh is being requested with different texture_params.\n\nmesh_id: '{}'\nrequested: '{}'\ncached: '{}'", id, texture_params, iter->second->get_texture_params());
			}
			return iter->second.get();
		}

		auto address = _resource_system.resolve_address("mesh", "meshes", RESOLVE_ADDRESS_MESH_EXTENSIONS, id.c_str(), id_source_description.c_str());
		bgfx_mesh* new_mesh = new bgfx_mesh(texture_params, address);
		_meshs[id] = std::unique_ptr<bgfx_mesh>(new_mesh);

		return new_mesh;
	}

	const resource_factory_caching_context& bgfx_mesh_factory::get_caching_context() const {
		return _caching_context;
	}

}
