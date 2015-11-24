#include "d3d9_font_renderer_shader_program_provider.h"

#include "solar/utility/assert.h"
#include "./shaders/font_normal_embedded_code.h"
#include "./shaders/font_dropshadow_embedded_code.h"

namespace solar {

	d3d9_font_renderer_shader_program_provider::d3d9_font_renderer_shader_program_provider(d3d9_shader_program_factory& factory) 
		: _factory(factory) 
		, _normal_shader_program(nullptr)
		, _dropshadow_shader_program(nullptr) {
	}

	d3d9_font_renderer_shader_program_provider::~d3d9_font_renderer_shader_program_provider() {
		ASSERT(_normal_shader_program == nullptr);
		ASSERT(_dropshadow_shader_program == nullptr);
	}

	void d3d9_font_renderer_shader_program_provider::setup() {
		_normal_shader_program = _factory.create_embedded_code_shader_program(FONT_NORMAL_EMBEDDED_CODE);
		_dropshadow_shader_program = _factory.create_embedded_code_shader_program(FONT_DROPSHADOW_EMBEDDED_CODE);
	}

	void d3d9_font_renderer_shader_program_provider::teardown() {
		_factory.release_embedded_code_shader_program(_normal_shader_program);
		_normal_shader_program = nullptr;

		_factory.release_embedded_code_shader_program(_dropshadow_shader_program);
		_dropshadow_shader_program = nullptr;
	}

	shader_program& d3d9_font_renderer_shader_program_provider::get_normal_shader_program() {
		ASSERT(_normal_shader_program != nullptr);
		return *_normal_shader_program;
	}

	shader_program& d3d9_font_renderer_shader_program_provider::get_dropshadow_shader_program() {
		ASSERT(_dropshadow_shader_program != nullptr);
		return *_dropshadow_shader_program;
	}

}