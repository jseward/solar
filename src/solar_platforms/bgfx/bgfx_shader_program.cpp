#include "bgfx_shader_program.h"

#include "solar/utility/unused_parameter.h"

namespace solar {

	bgfx_shader_program::bgfx_shader_program(const resource_address& resource_address)
		: _resource_address(resource_address) {
	}

	bgfx_shader_program::~bgfx_shader_program() {
	}

	const resource_address& bgfx_shader_program::get_resource_address() const {
		return _resource_address;
	}

	void bgfx_shader_program::start() {
		//todo
	}

	void bgfx_shader_program::stop() {
		//todo
	}

	void bgfx_shader_program::commit_param_changes() {
		//todo
	}

	void bgfx_shader_program::forget_param_changes() {
		//todo
	}

	bool bgfx_shader_program::set_bool(const char* name, bool value) {
		//todo
		UNUSED_PARAMETER(name);
		UNUSED_PARAMETER(value);
		return false;
	}

	bool bgfx_shader_program::set_float(const char* name, float value) {
		//todo
		UNUSED_PARAMETER(name);
		UNUSED_PARAMETER(value);
		return false;
	}

	bool bgfx_shader_program::set_float_array(const char* name, const float* values, unsigned int count) {
		//todo
		UNUSED_PARAMETER(name);
		UNUSED_PARAMETER(values);
		UNUSED_PARAMETER(count);
		return false;
	}

	bool bgfx_shader_program::set_mat44(const char* name, const mat44& value) {
		//todo
		UNUSED_PARAMETER(name);
		UNUSED_PARAMETER(value);
		return false;
	}

	bool bgfx_shader_program::set_texture(const char* name, texture& texture) {
		//todo
		UNUSED_PARAMETER(name);
		UNUSED_PARAMETER(texture);
		return false;
	}

	bool bgfx_shader_program::set_platform_texture(const char* name, void* texture) {
		//todo
		UNUSED_PARAMETER(name);
		UNUSED_PARAMETER(texture);
		return false;
	}

}
