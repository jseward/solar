#pragma once

#include <vector>
#include "solar/math/vec2.h"
#include "solar/math/vec3.h"
#include "solar/colors/color.h"

namespace solar {

	class viewport;
	class camera;

	class prim2d_lines {
	private:
		const unsigned int DEFAULT_CIRCLE_SEGMENT_COUNT = 16;

	private:
		std::vector<vec2> _points_buffer;

	public:
		prim2d_lines();
		virtual ~prim2d_lines();

		virtual bool is_rendering() const = 0;
		virtual void begin_rendering(float line_width) = 0;
		virtual void end_rendering() = 0;
		virtual void render_segments(const vec2* points, unsigned int point_count, const color& color) = 0;

		void render_segment(const vec2& p0, const vec2& p1, const color& color);
		void render_circle(const vec2& center, float radius, const color& color);
		void render_circle(const vec2& center, float radius, const color& color, float begin_rad, float end_rad);
		void render_circle(const vec2& center, float radius, const color& color, float begin_rad, float end_rad, unsigned int segment_count);
		void render_rect(const rect& area, const color& color);
		void render_polygon(const vec2* points, unsigned int point_count, const color& color);
	};

}