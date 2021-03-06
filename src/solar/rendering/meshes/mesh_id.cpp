#include "mesh_id.h"

namespace solar {

	mesh_factory* mesh_id::_s_factory = nullptr;

	mesh_id::mesh_id() {
	}

	const resource_factory_caching_context& mesh_id::get_factory_caching_context(const mesh_factory& factory) const {
		return factory.get_caching_context();
	}

	mesh* mesh_id::get_uncached_resource(mesh_factory& factory, const std::string& id, const std::string& id_source_description) const {
		return factory.get_mesh(id, id_source_description);
	}

}