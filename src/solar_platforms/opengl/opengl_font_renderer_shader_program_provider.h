#pragma once

#include "solar/rendering/fonts/font_renderer_shader_program_provider.h"

namespace solar {

	class opengl_shader_program_factory;
	class opengl_shader_program;

	class opengl_font_renderer_shader_program_provider : public font_renderer_shader_program_provider {
	private:
		opengl_shader_program_factory& _factory;

		opengl_shader_program* _normal_shader_program;
		opengl_shader_program* _dropshadow_shader_program;

	public:
		opengl_font_renderer_shader_program_provider(opengl_shader_program_factory& factory);
		virtual ~opengl_font_renderer_shader_program_provider();

		void setup();
		void teardown();

		virtual shader_program& get_normal_shader_program() override;
		virtual shader_program& get_dropshadow_shader_program() override;
	};


}