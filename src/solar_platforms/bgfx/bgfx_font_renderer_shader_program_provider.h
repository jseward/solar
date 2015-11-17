#pragma once

#include "solar/rendering/fonts/font_renderer_shader_program_provider.h"

namespace solar {

	class bgfx_shader_program_factory;
	class bgfx_shader_program;

	class bgfx_font_renderer_shader_program_provider : public font_renderer_shader_program_provider {
	private:
		bgfx_shader_program_factory& _factory;

		bgfx_shader_program* _normal_shader_program;
		bgfx_shader_program* _dropshadow_shader_program;

	public:
		bgfx_font_renderer_shader_program_provider(bgfx_shader_program_factory& factory);
		virtual ~bgfx_font_renderer_shader_program_provider();

		void setup();
		void teardown();

		virtual shader_program& get_normal_shader_program() override;
		virtual shader_program& get_dropshadow_shader_program() override;
	};


}