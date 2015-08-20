#include "shader_id.h"

namespace solar {

	shader_factory* shader_id::_s_factory = nullptr;

	shader_id::shader_id() {
	}

	int shader_id::get_current_resource_caching_context(const shader_factory& factory) const {
		return factory.get_shader_caching_context();
	}

	shader* shader_id::get_uncached_resource(shader_factory& factory, const std::string& id, const std::string& id_source_description) const {
		return factory.get_shader(id, id_source_description);
	}

}