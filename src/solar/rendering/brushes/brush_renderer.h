#pragma once

#include "brush_render_mode.h"

namespace solar {

	class prim2d;
	class brush;
	class rect;
	class color;
	class shader;
	class render_state_block;

	class brush_renderer {
	private:
		prim2d& _prim2d;

	public:
		brush_renderer(prim2d& prim2d);
		brush_renderer(const brush_renderer&) = delete;
		brush_renderer& operator=(const brush_renderer&) = delete;
		~brush_renderer();

		void begin_rendering(const rect& render_area, shader& shader, render_state_block* rs_block);
		void end_rendering();

		void render_brush(
			const brush& brush, 
			brush_render_mode render_mode, 
			const rect& area, 
			float area_scale, 
			const color& color);

		void render_brush_with_angle(
			const brush& brush, 
			const rect& area, 
			const color& color, 
			float angle);

	private:
		void render_brush_stretched(const brush& brush, const rect& area, float area_scale, const color& color);
		void render_brush_tiled(const brush& brush, const rect& area, float area_scale, const color& color);
	};

}