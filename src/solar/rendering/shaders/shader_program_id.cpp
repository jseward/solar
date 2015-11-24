#include "shader_program_id.h"

namespace solar {

	shader_program_factory* shader_program_id::_s_factory = nullptr;

	shader_program_id::shader_program_id() {
	}

	const resource_factory_caching_context& shader_program_id::get_factory_caching_context(const shader_program_factory& factory) const {
		return factory.get_shader_program_caching_context();
	}

	shader_program* shader_program_id::get_uncached_resource(shader_program_factory& factory, const std::string& id, const std::string& id_source_description) const {
		return factory.get_shader_program(id, id_source_description);
	}

}