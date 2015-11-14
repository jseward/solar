#pragma once

#include <string>
#include "solar/resources/resource_id.h"
#include "shader_program.h"
#include "shader_program_factory.h"

namespace solar {

	class shader_program_id : public resource_id<shader_program, shader_program_factory> {
	public:
		shader_program_id();
		virtual const resource_factory_caching_context& get_factory_caching_context(const shader_program_factory& factory) const override;
		virtual shader_program* get_uncached_resource(shader_program_factory& factory, const std::string& id, const std::string& id_source_description) const override;
	};

}