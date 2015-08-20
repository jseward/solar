#pragma once

#include <string>
#include "shader.h"

namespace solar {

	class shader_factory {
	public:
		virtual void set_render_states_to_defaults() = 0;
		virtual shader* get_shader(const std::string& id, const std::string& id_source_description) = 0;
		virtual int get_shader_caching_context() const = 0;
	};

}