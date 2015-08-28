#include "prim2d.h"

#include "solar/math/point_helpers.h"
#include "solar/math/math_helpers.h"

namespace solar {

	void prim2d::render_rect(const rect& area, const color& color) {
		render_rect(
			point_to_vec2(area.get_top_left()),
			point_to_vec2(area.get_top_right()),
			point_to_vec2(area.get_bottom_right()),
			point_to_vec2(area.get_bottom_left()),
			color,
			simple_rect_uvs());
	}

	void prim2d::render_rect(const rect& area, const color& color, const simple_rect_uvs& uvs) {
		render_rect(
			point_to_vec2(area.get_top_left()),
			point_to_vec2(area.get_top_right()),
			point_to_vec2(area.get_bottom_right()),
			point_to_vec2(area.get_bottom_left()),
			color,
			uvs);
	}

	void prim2d::render_circle(const vec2& center, float radius, const color& color) {
		render_circle(center, radius, color, 16);
	}

	void prim2d::render_circle(const vec2& center, float radius, const color& color, int segment_count) {
		const deg deg_inc = deg(360.f / segment_count);
		const float sin_inc = sin(deg_inc);
		const float cos_inc = cos(deg_inc);

		vec2 r = vec2(1.f, 0.f);
		vec2 prev_segment_point = center + (r * radius);
		for (int i = 0; i < segment_count; ++i) {
			r = vec2(
				(r._x * cos_inc) - (r._y * sin_inc),
				(r._x * sin_inc) + (r._y * cos_inc));
			vec2 curr_segment_point = center + (r * radius);

			render_tri(prev_segment_point, curr_segment_point, center, color);

			prev_segment_point = curr_segment_point;
		}
	}

	void prim2d::render_polygon(const vec2* points, unsigned int point_count, const color& color) {
		for (unsigned int i = 1; i < point_count - 1; i++) {
			render_tri(
				points[0], 
				points[i], 
				points[i + 1], 
				color);
		}
	}

}