#include "opengl_mesh_factory.h"

#include "solar/utility/assert.h"
#include "solar/utility/alert.h"
#include "solar/resources/resource_system.h"
#include "solar/rendering/meshes/mesh_constants.h"
#include "opengl_mesh.h"

namespace solar {

	opengl_mesh_factory::opengl_mesh_factory(resource_system& resource_system)
		: _resource_system(resource_system)
		, _is_setup(false) {
	}

	opengl_mesh_factory::~opengl_mesh_factory() {
		ASSERT(!_is_setup);
	}

	void opengl_mesh_factory::setup() {
		ASSERT(!_is_setup);
		_is_setup = true;
	}

	void opengl_mesh_factory::teardown() {
		remove_all_meshs();
		_is_setup = false;
	}

	void opengl_mesh_factory::remove_all_meshs() {
		_meshs.clear();
		_caching_context.increment();
	}

	mesh* opengl_mesh_factory::get_mesh(const std::string& id, const std::string& id_source_description) {
		ASSERT(_is_setup);

		auto iter = _meshs.find(id);
		if (iter != _meshs.end()) {
			return iter->second.get();
		}

		auto address = _resource_system.resolve_address_to_file("mesh", "meshes", RESOLVE_ADDRESS_MESH_EXTENSIONS, id.c_str(), id_source_description.c_str());
		opengl_mesh* new_mesh = new opengl_mesh(address);
		_meshs[id] = std::unique_ptr<opengl_mesh>(new_mesh);

		return new_mesh;
	}

	const resource_factory_caching_context& opengl_mesh_factory::get_caching_context() const {
		return _caching_context;
	}

}
