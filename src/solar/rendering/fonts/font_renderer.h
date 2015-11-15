#pragma once

#include "font.h"
#include "font_render_params.h"
#include "font_renderer_line_builder.h"
#include "font_renderer_shader_program_provider.h"
#include "solar/math/rect.h"
#include "solar/colors/color.h"
#include "solar/rendering/render_states/render_state_block_ptr.h"

namespace solar {

	class resource_system;
	class prim2d;

	class font_renderer {
	private:
		resource_system& _resource_system;
		prim2d& _prim2d;
		render_device& _render_device;
		font_renderer_shader_program_provider& _shader_program_provider;

		render_state_block_ptr _render_state_block;
		font_renderer_line_builder _line_builder;

	public:
		font_renderer(render_device& render_device, resource_system& resource_system, prim2d& prim2d, font_renderer_shader_program_provider& shader_program_provider);
		font_renderer(const font_renderer&) = delete;
		font_renderer& operator=(const font_renderer&) = delete;
		~font_renderer();

		void setup();
		void teardown();

		void begin_rendering(const rect& viewport_area);
		void end_rendering();
		void render(const font_render_params& params);

		int calculate_render_height(const font_render_params& params);

	private:
		void render_lines(const font_render_params& params, const std::vector<font_renderer_line>& lines, const color& color);
		float calculate_base_line_error(const font& font, float scale) const;
		void set_dropshadow_shader(const font_render_params& params);
		void set_normal_shader(const font_render_params& params);
	};

}