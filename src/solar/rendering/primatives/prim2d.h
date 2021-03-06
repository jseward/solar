#pragma once

#include <vector>
#include "solar/math/rect.h"
#include "solar/math/rectf.h"
#include "solar/math/vec2.h"
#include "solar/colors/color.h"
#include "solar/rendering/textures/simple_rect_uvs.h"

namespace solar {

	class shader_program;
	class texture;
	class viewport;
	class camera;
	class render_state_block;

	class prim2d {
	private:
		const unsigned int DEFAULT_CIRCLE_SEGMENT_COUNT = 16;

	public:
		virtual void begin_rendering(const rect& viewport_area) = 0;
		virtual void begin_rendering(const rect& viewport_area, shader_program& program, render_state_block* rs_block) = 0;
		virtual void end_rendering() = 0;
		virtual void set_shader_program(shader_program& program) = 0;
		virtual void set_texture(texture* texture) = 0;
		virtual void render_rect(const vec2& top_left, const vec2& top_right, const vec2& bottom_right, const vec2& bottom_left, const color& color, const simple_rect_uvs& uvs) = 0;
		virtual void render_triangle(const vec2& p0, const uv& uv0, const vec2& p1, const uv& uv1, const vec2& p2, const uv& uv2, const color& color) = 0;

	public:
		void render_rect(const rect& area, const color& color);
		void render_rect(const rect& area, const color& color, const simple_rect_uvs& uvs);
		void render_rect(const rectf& area, const color& color);
		void render_rect_with_angle(const rect& area, const color& color, const simple_rect_uvs& uvs, float angle);
		void render_rect_with_angle(const rectf& area, const color& color, const simple_rect_uvs& uvs, float angle);
		void render_circle(const vec2& center, float radius, const color& color);
		void render_circle(const vec2& center, float radius, const color& color, float begin_rad, float end_rad);
		void render_circle(const vec2& center, float radius, const color& color, float begin_rad, float end_rad, unsigned int segment_count);
		void render_polygon(const vec2* points, unsigned int point_count, const color& color);
	};

}
