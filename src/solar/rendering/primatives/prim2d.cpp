#include "prim2d.h"

#include "solar/math/math_type_convert.h"
#include "solar/math/math_helpers.h"
#include "solar/math/math_constants.h"
#include "solar/rendering/viewport.h"
#include "solar/rendering/cameras/camera.h"

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

	void prim2d::render_rect(const rectf& area, const color& color) {
		render_rect(
			pointf_to_vec2(area.get_top_left()),
			pointf_to_vec2(area.get_top_right()),
			pointf_to_vec2(area.get_bottom_right()),
			pointf_to_vec2(area.get_bottom_left()),
			color,
			simple_rect_uvs());
	}

	void prim2d::render_rect_with_angle(const rect& area, const color& color, const simple_rect_uvs& uvs, float angle) {
		render_rect_with_angle(rect_to_rectf(area), color, uvs, angle);
	}

	void prim2d::render_rect_with_angle(const rectf& area, const color& color, const simple_rect_uvs& uvs, float angle) {
		const pointf center = area.get_center();
		
		//Need to translate center to 0,0 so the rotation works correctly...
		const mat44 rotate = make_mat44_rotation_on_z(angle);
		const mat44 translate = make_mat44_translation(-center._x, -center._y, 0.0f);
		const mat44 transform = translate * rotate;

		const vec3 inverse_translation = vec3(center._x, center._y, 0.f);
		const vec3 rotated_top_left = inverse_translation + (vec3(area.get_left(), area.get_top(), 0.f) * transform);
		const vec3 rotated_top_right = inverse_translation + (vec3(area.get_right(), area.get_top(), 0.f) * transform);
		const vec3 rotated_bottom_right = inverse_translation + (vec3(area.get_right(), area.get_bottom(), 0.f) * transform);
		const vec3 rotated_bottom_left = inverse_translation + (vec3(area.get_left(), area.get_bottom(), 0.f) * transform);

		render_rect(
			vec2(rotated_top_left._x, rotated_top_left._y),
			vec2(rotated_top_right._x, rotated_top_right._y),
			vec2(rotated_bottom_right._x, rotated_bottom_right._y),
			vec2(rotated_bottom_left._x, rotated_bottom_left._y),
			color,
			uvs);
	}

	void prim2d::render_circle(const vec2& center, float radius, const color& color) {
		render_circle(center, radius, color, 0.f, TWO_PI, DEFAULT_CIRCLE_SEGMENT_COUNT);
	}

	void prim2d::render_circle(const vec2& center, float radius, const color& color, float begin_rad, float end_rad) {
		render_circle(center, radius, color, begin_rad, end_rad, DEFAULT_CIRCLE_SEGMENT_COUNT);
	}

	void prim2d::render_circle(const vec2& center, float radius, const color& color, float begin_rad, float end_rad, unsigned int segment_count) {
		ASSERT(segment_count >= 3);
		ASSERT(begin_rad <= end_rad);

		const float rad_inc = (end_rad - begin_rad) / segment_count;
		const float sin_inc = sin(rad_inc);
		const float cos_inc = cos(rad_inc);

		vec2 r = vec2(cos(begin_rad), sin(begin_rad));
		vec2 prev_segment_point = center + (vec2(r._x, -r._y) * radius);
		for (unsigned int i = 0; i < segment_count; ++i) {
			r = vec2(
				(r._x * cos_inc) - (r._y * sin_inc),
				(r._x * sin_inc) + (r._y * cos_inc));
			vec2 curr_segment_point = center + (vec2(r._x, -r._y) * radius);

			//todo - calculate proper uvs
			render_triangle(center, uv(), curr_segment_point, uv(), prev_segment_point, uv(), color);

			prev_segment_point = curr_segment_point;
		}
	}

	void prim2d::render_polygon(const vec2* points, unsigned int point_count, const color& color) {
		for (unsigned int i = 1; i < point_count - 1; i++) {
			render_triangle(
				points[0], 
				uv(),
				points[i], 
				uv(),
				points[i + 1], 
				uv(),
				color);
		}
	}

}
