#include "opengl_shader_program.h"

#include "solar/utility/assert.h"
#include "solar/utility/verify.h"
#include "solar/utility/unused_parameter.h"
#include "solar/strings/string_build.h"
#include "solar/resources/resource_system.h"
#include "opengl_context.h"
#include "opengl_shader_program_factory.h"

namespace solar {

	opengl_shader_program::opengl_shader_program(opengl_shader_program_factory& factory, const resource_address& resource_address)
		: _factory(factory)
		, _resource_address(resource_address) {
	}

	opengl_shader_program::opengl_shader_program(opengl_shader_program_factory& factory, const char* vs_embedded_code, const char* fs_embedded_code)
		: _factory(factory) {
		ASSERT(false);//todo
		UNUSED_PARAMETER(vs_embedded_code);
		UNUSED_PARAMETER(fs_embedded_code);
	}

	opengl_shader_program::~opengl_shader_program() {
	}

	const resource_address& opengl_shader_program::get_resource_address() const {
		return _resource_address;
	}

	void opengl_shader_program::load() {
		ASSERT(false);//todo
	}

	void opengl_shader_program::create_embedded() {
		ASSERT(false);//todo
	}

	void opengl_shader_program::start() {
		//null-op of opengl?
	}

	void opengl_shader_program::stop() {
		//null-op of opengl?
	}

	void opengl_shader_program::commit_param_changes() {
		//todo
	}

	void opengl_shader_program::forget_param_changes() {
		//todo
	}

	bool opengl_shader_program::set_bool(const char* name, bool value) {
		//todo
		UNUSED_PARAMETER(name);
		UNUSED_PARAMETER(value);
		return false;
	}

	bool opengl_shader_program::set_float(const char* name, float value) {
		//todo
		UNUSED_PARAMETER(name);
		UNUSED_PARAMETER(value);
		return false;
	}

	bool opengl_shader_program::set_float_array(const char* name, const float* values, unsigned int count) {
		//todo
		UNUSED_PARAMETER(name);
		UNUSED_PARAMETER(values);
		UNUSED_PARAMETER(count);
		return false;
	}

	bool opengl_shader_program::set_mat44(const char* name, const mat44& value) {
		//todo
		UNUSED_PARAMETER(name);
		UNUSED_PARAMETER(value);
		return false;
	}

	bool opengl_shader_program::set_texture(const char* name, texture& texture) {
		//todo
		UNUSED_PARAMETER(name);
		UNUSED_PARAMETER(texture);
		return false;
	}

	bool opengl_shader_program::set_platform_texture(const char* name, void* texture) {
		//todo
		UNUSED_PARAMETER(name);
		UNUSED_PARAMETER(texture);
		return false;
	}
	
}
