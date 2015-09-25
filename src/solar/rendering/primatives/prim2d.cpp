#include "prim2d.h"

#include "solar/math/point_helpers.h"
#include "solar/math/math_helpers.h"
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

	void prim2d::render_circle(const vec2& center, float radius, const color& color) {
		render_circle(center, radius, color, 16);
	}

	void prim2d::render_circle(const vec2& center, float radius, const color& color, unsigned int segment_count) {
		const deg deg_inc = deg(360.f / segment_count);
		const float sin_inc = sin(deg_inc);
		const float cos_inc = cos(deg_inc);

		vec2 r = vec2(1.f, 0.f);
		vec2 prev_segment_point = center + (r * radius);
		for (unsigned int i = 0; i < segment_count; ++i) {
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

	void prim2d::render_world_polygon(const viewport& viewport, const camera& camera, const vec3* points, unsigned int point_count, const color& color) {
		auto screen_points = _world_projection_buffer.project_points(viewport, camera, points, point_count);
		if (!screen_points.empty()) {
			render_polygon(screen_points.data(), screen_points.size(), color);
		}
	}

	void prim2d::render_world_circle(const viewport& viewport, const camera& camera, const vec3& center, float radius, const color& color) {
		render_world_circle(viewport, camera, center, radius, color, DEFAULT_CIRCLE_SEGMENT_COUNT);
	}

	void prim2d::render_world_circle(const viewport& viewport, const camera& camera, const vec3& center, float radius, const color& color, unsigned int segment_count) {
		vec3 radius_marker = center + (camera.get_basis().get_rotation().get_up() * radius);

		vec2 screen_center;
		vec2 screen_radius_marker;
		if (
			viewport.try_project(screen_center, camera.get_view_projection_transform(), center) &&
			viewport.try_project(screen_radius_marker, camera.get_view_projection_transform(), radius_marker)) {

			const float screen_radius = get_distance(screen_center, screen_radius_marker);
			render_circle(screen_center, screen_radius, color, segment_count);
		}
	}

	void prim2d::render_world_indexed_tris(const viewport& viewport, const camera& camera, const vec3* vertices, unsigned int vertex_count, const unsigned short* indices, unsigned int index_count, const color& color) {
		auto screen_points = _world_projection_buffer.project_points(viewport, camera, vertices, vertex_count);
		if (!screen_points.empty()) {
			render_indexed_tris(screen_points.data(), screen_points.size(), indices, index_count, color);
		}
	}

}
