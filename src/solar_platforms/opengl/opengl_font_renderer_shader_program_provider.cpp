#include "opengl_font_renderer_shader_program_provider.h"

#include "solar/utility/assert.h"
#include "opengl_shader_program_factory.h"

namespace solar {

	opengl_font_renderer_shader_program_provider::opengl_font_renderer_shader_program_provider(opengl_shader_program_factory& factory)
		: _factory(factory) 
		, _normal_shader_program(nullptr) 
		, _dropshadow_shader_program(nullptr) {
	}

	opengl_font_renderer_shader_program_provider::~opengl_font_renderer_shader_program_provider() {
		ASSERT(_normal_shader_program == nullptr);
		ASSERT(_dropshadow_shader_program == nullptr);
	}

	void opengl_font_renderer_shader_program_provider::setup() {
		ASSERT(false);//todo
		//auto renderer_type = opengl::getRendererType();

		//_normal_shader_program = _factory.create_embedded_code_shader_program(
		//	get_font_normal_vs_embedded_code(renderer_type), 
		//	get_font_normal_fs_embedded_code(renderer_type));
		//
		//_dropshadow_shader_program = _factory.create_embedded_code_shader_program(
		//	get_font_dropshadow_vs_embedded_code(renderer_type), 
		//	get_font_dropshadow_fs_embedded_code(renderer_type));
	}

	void opengl_font_renderer_shader_program_provider::teardown() {
		ASSERT(false);//todo
		//_factory.release_embedded_code_shader_program(_normal_shader_program);
		//_normal_shader_program = nullptr;

		//_factory.release_embedded_code_shader_program(_dropshadow_shader_program);
		//_dropshadow_shader_program = nullptr;
	}

	shader_program& opengl_font_renderer_shader_program_provider::get_normal_shader_program() {
		ASSERT(_normal_shader_program != nullptr);
		return *_normal_shader_program;
	}

	shader_program& opengl_font_renderer_shader_program_provider::get_dropshadow_shader_program() {
		ASSERT(_dropshadow_shader_program != nullptr);
		return *_dropshadow_shader_program;
	}

}
