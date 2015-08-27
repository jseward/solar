#include "prim2d_lines.h"

#include "solar/utility/assert.h"
#include "solar/math/math_helpers.h"

namespace solar {

	void prim2d_lines::render_segment(const vec2& p0, const vec2& p1, const color& color) {
		vec2 points[2] = { p0, p1 };
		render_segments(points, 2, color);
	}

	void prim2d_lines::render_circle(const vec2& center, float radius, const color& color) {
		render_circle(center, radius, color, 16);
	}

	void prim2d_lines::render_circle(const vec2& center, float radius, const color& color, int segment_count) {
		_circle_segment_points.clear();
		_circle_segment_points.reserve(segment_count);

		const deg deg_inc = deg(360.f / segment_count);
		const float sin_inc = sin(deg_inc);
		const float cos_inc = cos(deg_inc);

		vec2 r = vec2(1.f, 0.f);
		for (int i = 0; i < segment_count; ++i) {
			vec2 segment_point = center + (r * radius);
			_circle_segment_points.push_back(segment_point);
			r = vec2(
				(r._x * cos_inc) - (r._y * sin_inc),
				(r._x * sin_inc) + (r._y * cos_inc));
		}

		render_segments(_circle_segment_points.data(), segment_count, color);
	}

}