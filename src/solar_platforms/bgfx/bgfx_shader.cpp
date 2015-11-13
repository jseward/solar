#include "bgfx_shader.h"

#include "solar/utility/unused_parameter.h"

namespace solar {

	bgfx_shader::bgfx_shader(const resource_address& resource_address)
		: _resource_address(resource_address) {
	}

	bgfx_shader::~bgfx_shader() {
	}

	const resource_address& bgfx_shader::get_resource_address() const {
		return _resource_address;
	}

	void bgfx_shader::start() {
		//todo
	}

	void bgfx_shader::stop() {
		//todo
	}

	void bgfx_shader::commit_param_changes() {
		//todo
	}

	void bgfx_shader::forget_param_changes() {
		//todo
	}

	bool bgfx_shader::set_bool(const char* name, bool value) {
		//todo
		UNUSED_PARAMETER(name);
		UNUSED_PARAMETER(value);
		return false;
	}

	bool bgfx_shader::set_float(const char* name, float value) {
		//todo
		UNUSED_PARAMETER(name);
		UNUSED_PARAMETER(value);
		return false;
	}

	bool bgfx_shader::set_float_array(const char* name, const float* values, unsigned int count) {
		//todo
		UNUSED_PARAMETER(name);
		UNUSED_PARAMETER(values);
		UNUSED_PARAMETER(count);
		return false;
	}

	bool bgfx_shader::set_mat44(const char* name, const mat44& value) {
		//todo
		UNUSED_PARAMETER(name);
		UNUSED_PARAMETER(value);
		return false;
	}

	bool bgfx_shader::set_texture(const char* name, texture& texture) {
		//todo
		UNUSED_PARAMETER(name);
		UNUSED_PARAMETER(texture);
		return false;
	}

	bool bgfx_shader::set_platform_texture(const char* name, void* texture) {
		//todo
		UNUSED_PARAMETER(name);
		UNUSED_PARAMETER(texture);
		return false;
	}

}
