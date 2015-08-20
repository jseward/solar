#pragma once

#include <string>
#include "solar/resources/resource_id.h"
#include "shader.h"
#include "shader_factory.h"

namespace solar {

	class shader_id : public resource_id<shader, shader_factory> {
	public:
		shader_id();
		virtual int get_current_resource_caching_context(const shader_factory& factory) const override;
		virtual shader* get_uncached_resource(shader_factory& factory, const std::string& id, const std::string& id_source_description) const override;
	};

}