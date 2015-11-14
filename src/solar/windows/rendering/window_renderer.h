#pragma once

#include "solar/rendering/brushes/brush_render_mode.h"
#include "solar/rendering/shaders/shader_program_id.h"
#include "solar/colors/color.h"
#include "solar/windows/window_font_def.h"
#include "window_renderer_def.h"

namespace solar {

	class window;
	class window_layout;
	class prim2d;
	class prim2d_lines;
	class font_renderer;
	class brush;
	class brush_renderer;
	class resource_system;

	class window_renderer {

	private:
		resource_system& _resource_system;
		prim2d& _prim2d;
		prim2d_lines& _prim2d_lines;
		font_renderer& _font_renderer;
		brush_renderer& _brush_renderer;
		render_device& _render_device;

		window_renderer_def _def;
		render_state_block_ptr _render_state_block;

	public:
		window_renderer(render_device& render_device, resource_system& resource_system, prim2d& prim2d, prim2d_lines& prim2d_lines, font_renderer& font_renderer, brush_renderer& brush_renderer);
		window_renderer(const window_renderer&) = delete;
		window_renderer& operator=(const window_renderer&) = delete;
		~window_renderer();

		void setup();
		void teardown();

		prim2d& get_prim2d();
		prim2d_lines& get_prim2d_lines();
		brush_renderer& get_brush_renderer();
		shader_program& get_brush_shader_program();

		void begin_brush_rendering();
		void end_brush_rendering();
		void render_brush(const window& window, const brush* brush);
		void render_brush(const window& window, const brush* brush, brush_render_mode render_mode);
		void render_brush(const window& window, const brush* brush, brush_render_mode render_mode, const window_layout& layout);
		void render_brush(const window& window, const brush* brush, brush_render_mode render_mode, const rect& render_area);

		void begin_font_rendering();
		void end_font_rendering();
		void render_font(const window& window, const window_font_def& font_def, const wchar_t* text);
		void render_font(const window& window, const window_font_def& font_def, const wchar_t* text, const rect& render_area);

		color get_default_brush_color(const window& window) const;
	};


}