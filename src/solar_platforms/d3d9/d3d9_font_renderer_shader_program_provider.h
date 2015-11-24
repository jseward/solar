#pragma once

#include "d3d9_shader_program_factory.h"
#include "solar/rendering/fonts/font_renderer_shader_program_provider.h"

namespace solar {

	class d3d9_font_renderer_shader_program_provider : public font_renderer_shader_program_provider {
	private:
		d3d9_shader_program_factory& _factory;

		d3d9_shader_program* _normal_shader_program;
		d3d9_shader_program* _dropshadow_shader_program;

	public:
		d3d9_font_renderer_shader_program_provider(d3d9_shader_program_factory& factory);
		virtual ~d3d9_font_renderer_shader_program_provider();

		void setup();
		void teardown();

		shader_program& get_normal_shader_program() override;
		shader_program& get_dropshadow_shader_program() override;
	};

}