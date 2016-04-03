#include "prim2d_lines.h"

#include "solar/utility/assert.h"
#include "solar/utility/verify.h"
#include "solar/math/math_helpers.h"
#include "solar/math/math_constants.h"
#include "solar/math/math_type_convert.h"
#include "solar/rendering/viewport.h"
#include "solar/rendering/cameras/camera.h"

namespace solar {

	prim2d_lines::prim2d_lines() {
	}

	prim2d_lines::~prim2d_lines() {
	}

	void prim2d_lines::render_segment(const vec2& p0, const vec2& p1, const color& color) {
		vec2 points[2] = { p0, p1 };
		render_segments(points, 2, color);
	}

	void prim2d_lines::render_polygon(const vec2* points, unsigned int point_count, const color& color) {
		render_segments(points, point_count, color);
		if (point_count > 1) {
			render_segment(points[point_count - 1], points[0], color);
		}
	}

	void prim2d_lines::render_rect(const rect& area, const color& color) {
		vec2 points[5] = { 
			point_to_vec2(area.get_top_left()),
			point_to_vec2(area.get_top_right()),
			point_to_vec2(area.get_bottom_right()),
			point_to_vec2(area.get_bottom_left()),
			point_to_vec2(area.get_top_left())
		};		
		render_segments(points, 5, color);
	}

	void prim2d_lines::render_circle(const vec2& center, float radius, const color& color) {
		render_circle(center, radius, color, 0.f, TWO_PI, DEFAULT_CIRCLE_SEGMENT_COUNT);
	}

	void prim2d_lines::render_circle(const vec2& center, float radius, const color& color, float begin_rad, float end_rad) {
		render_circle(center, radius, color, begin_rad, end_rad, DEFAULT_CIRCLE_SEGMENT_COUNT);
	}

	void prim2d_lines::render_circle(const vec2& center, float radius, const color& color, float begin_rad, float end_rad, unsigned int segment_count) {
		ASSERT(segment_count >= 3);
		ASSERT(begin_rad <= end_rad);

		unsigned int point_count = segment_count + 1;

		_points_buffer.clear();
		_points_buffer.reserve(point_count);

		const float rad_inc = (end_rad - begin_rad) / segment_count;
		const float sin_inc = sin(rad_inc);
		const float cos_inc = cos(rad_inc);

		vec2 r = vec2(cos(begin_rad), sin(begin_rad));
		for (unsigned int i = 0; i < point_count; ++i) {
			vec2 segment_point = center + (vec2(r._x, -r._y) * radius);
			_points_buffer.push_back(segment_point);
			r = vec2(
				(r._x * cos_inc) - (r._y * sin_inc),
				(r._x * sin_inc) + (r._y * cos_inc));
		}

		render_segments(_points_buffer.data(), _points_buffer.size(), color);
	}

}
