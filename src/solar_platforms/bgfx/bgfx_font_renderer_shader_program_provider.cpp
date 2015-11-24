#include "bgfx_font_renderer_shader_program_provider.h"

#include "solar/utility/assert.h"
#include "bgfx_shader_program_factory.h"
#include "./shaders/font_shaders_embedded_code.h"

namespace solar {

	bgfx_font_renderer_shader_program_provider::bgfx_font_renderer_shader_program_provider(bgfx_shader_program_factory& factory)
		: _factory(factory) 
		, _normal_shader_program(nullptr) 
		, _dropshadow_shader_program(nullptr) {
	}

	bgfx_font_renderer_shader_program_provider::~bgfx_font_renderer_shader_program_provider() {
		ASSERT(_normal_shader_program == nullptr);
		ASSERT(_dropshadow_shader_program == nullptr);
	}

	void bgfx_font_renderer_shader_program_provider::setup() {
		auto renderer_type = bgfx::getRendererType();

		_normal_shader_program = _factory.create_embeded_code_shader_program(
			get_font_normal_vs_embedded_code(renderer_type), 
			get_font_normal_fs_embedded_code(renderer_type));
		
		_dropshadow_shader_program = _factory.create_embeded_code_shader_program(
			get_font_dropshadow_vs_embedded_code(renderer_type), 
			get_font_dropshadow_fs_embedded_code(renderer_type));
	}

	void bgfx_font_renderer_shader_program_provider::teardown() {
		_factory.release_embeded_code_shader_program(_normal_shader_program);
		_normal_shader_program = nullptr;

		_factory.release_embeded_code_shader_program(_dropshadow_shader_program);
		_dropshadow_shader_program = nullptr;
	}

	shader_program& bgfx_font_renderer_shader_program_provider::get_normal_shader_program() {
		ASSERT(_normal_shader_program != nullptr);
		return *_normal_shader_program;
	}

	shader_program& bgfx_font_renderer_shader_program_provider::get_dropshadow_shader_program() {
		ASSERT(_dropshadow_shader_program != nullptr);
		return *_dropshadow_shader_program;
	}

}