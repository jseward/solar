#pragma once

namespace solar {

	class shader_program;

	class font_renderer_shader_program_provider {
	public:
		virtual shader_program& get_normal_shader_program() = 0;
		virtual shader_program& get_dropshadow_shader_program() = 0;
	};

}