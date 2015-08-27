#pragma once

#include <vector>
#include "solar/math/vec2.h"
#include "solar/rendering/color.h"

namespace solar {

	class prim2d_lines {
	private:
		std::vector<vec2> _circle_segment_points;

	public:
		virtual bool is_rendering() const = 0;
		virtual void begin_rendering(float line_width) = 0;
		virtual void end_rendering() = 0;
		virtual void render_segments(const vec2* points, unsigned int point_count, const color& color) = 0;

		void render_segment(const vec2& p0, const vec2& p1, const color& color);
		void render_circle(const vec2& center, float radius, const color& color);
		void render_circle(const vec2& center, float radius, const color& color, int segment_count);
	};

}