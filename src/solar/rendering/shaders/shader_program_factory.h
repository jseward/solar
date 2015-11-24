#pragma once

#include <string>
#include "solar/resources/resource_factory_caching_context.h"
#include "shader_program.h"

namespace solar {

	class shader_program_factory {
	public:
		virtual shader_program* get_shader_program(const std::string& id, const std::string& id_source_description) = 0;
		virtual const resource_factory_caching_context& get_shader_program_caching_context() const = 0;
	};

}