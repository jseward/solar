#include "prim2d_lines.h"

#include "solar/utility/assert.h"
#include "solar/utility/verify.h"
#include "solar/math/math_helpers.h"
#include "solar/rendering/viewport.h"
#include "solar/rendering/cameras/camera.h"

namespace solar {

	void prim2d_lines::render_segment(const vec2& p0, const vec2& p1, const color& color) {
		vec2 points[2] = { p0, p1 };
		render_segments(points, 2, color);
	}

	void prim2d_lines::render_segments_looped(const vec2* points, unsigned int point_count, const color& color) {
		IF_VERIFY(point_count > 1) {
			render_segments(points, point_count, color);
			render_segment(points[point_count - 1], points[0], color);
		}
	}

	void prim2d_lines::render_circle(const vec2& center, float radius, const color& color) {
		render_circle(center, radius, color, DEFAULT_CIRCLE_SEGMENT_COUNT);
	}

	void prim2d_lines::render_circle(const vec2& center, float radius, const color& color, unsigned int segment_count) {
		ASSERT(segment_count > 0);

		_points_buffer.clear();
		_points_buffer.reserve(segment_count + 1);

		const deg deg_inc = deg(360.f / segment_count);
		const float sin_inc = sin(deg_inc);
		const float cos_inc = cos(deg_inc);

		vec2 r = vec2(1.f, 0.f);
		for (unsigned int i = 0; i < segment_count; ++i) {
			vec2 segment_point = center + (r * radius);
			_points_buffer.push_back(segment_point);
			r = vec2(
				(r._x * cos_inc) - (r._y * sin_inc),
				(r._x * sin_inc) + (r._y * cos_inc));
		}
		_points_buffer.push_back(_points_buffer[0]);

		render_segments(_points_buffer.data(), _points_buffer.size(), color);
	}

	void prim2d_lines::render_world_segment(const viewport& viewport, const camera& camera, const vec3& p0, const vec3& p1, const color& color) {
		vec2 screen_points[2];
		if (
			viewport.try_project(screen_points[0], camera.get_view_projection_transform(), p0) &&
			viewport.try_project(screen_points[1], camera.get_view_projection_transform(), p1)) {
			render_segments(screen_points, 2, color);
		}
	}

	void prim2d_lines::render_world_segments(const viewport& viewport, const camera& camera, const vec3* points, unsigned int point_count, const color& color) {
		if (try_project_points_to_points_buffer(viewport, camera, points, point_count)) {
			render_segments(_points_buffer.data(), _points_buffer.size(), color);
		}
	}

	void prim2d_lines::render_world_segments_looped(const viewport& viewport, const camera& camera, const vec3* points, unsigned int point_count, const color& color) {
		if (try_project_points_to_points_buffer(viewport, camera, points, point_count)) {
			render_segments_looped(_points_buffer.data(), _points_buffer.size(), color);
		}
	}

	void prim2d_lines::render_world_circle(const viewport& viewport, const camera& camera, const vec3& center, float radius, const color& color) {
		render_world_circle(viewport, camera, center, radius, color, DEFAULT_CIRCLE_SEGMENT_COUNT);
	}

	void prim2d_lines::render_world_circle(const viewport& viewport, const camera& camera, const vec3& center, float radius, const color& color, unsigned int segment_count) {
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

	bool prim2d_lines::try_project_points_to_points_buffer(const viewport& viewport, const camera& camera, const vec3* points, unsigned int point_count) {
		_points_buffer.clear();
		_points_buffer.reserve(point_count);
		for (unsigned int i = 0; i < point_count; ++i) {
			vec2 screen_point;
			if (!viewport.try_project(screen_point, camera.get_view_projection_transform(), points[i])) {
				return false;
			}
			_points_buffer.push_back(screen_point);
		}
		return true;
	}

}