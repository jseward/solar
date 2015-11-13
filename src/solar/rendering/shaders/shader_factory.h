#pragma once

#include <string>
#include "solar/resources/resource_factory_caching_context.h"
#include "shader.h"

namespace solar {

	class shader_factory {
	public:
		virtual shader* get_shader(const std::string& id, const std::string& id_source_description) = 0;
		virtual const resource_factory_caching_context& get_caching_context() const = 0;
	};

}