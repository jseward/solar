#include "prim2d_lines.h"

#include "solar/utility/assert.h"
#include "solar/utility/verify.h"
#include "solar/math/math_helpers.h"
#include "solar/math/math_constants.h"
#include "solar/rendering/viewport.h"
#include "solar/rendering/cameras/camera.h"

namespace solar {

	void prim2d_lines::render_segment(const vec2& p0, const vec2& p1, const color& color) {
		vec2 points[2] = { p0, p1 };
		render_segments(points, 2, color);
	}

	void prim2d_lines::render_segments_looped(const vec2* points, unsigned int point_count, const color& color) {
		if (point_count > 1) {
			render_segments(points, point_count, color);
			render_segment(points[point_count - 1], points[0], color);
		}
	}

	void prim2d_lines::render_circle(const vec2& center, float radius, const color& color) {
		render_circle(center, radius, color, DEFAULT_CIRCLE_SEGMENT_COUNT);
	}

	void prim2d_lines::render_circle(const vec2& center, float radius, const color& color, unsigned int segment_count) {
		ASSERT(segment_count > 0);

		_render_buffer.clear();
		_render_buffer.reserve(segment_count + 1);

		const float rad_inc = TWO_PI / segment_count;
		const float sin_inc = sin(rad_inc);
		const float cos_inc = cos(rad_inc);

		vec2 r = vec2(1.f, 0.f);
		for (unsigned int i = 0; i < segment_count; ++i) {
			vec2 segment_point = center + (r * radius);
			_render_buffer.push_back(segment_point);
			r = vec2(
				(r._x * cos_inc) - (r._y * sin_inc),
				(r._x * sin_inc) + (r._y * cos_inc));
		}
		_render_buffer.push_back(_render_buffer[0]);

		render_segments(_render_buffer.data(), _render_buffer.size(), color);
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
		auto screen_points = _world_projection_buffer.project_points(viewport, camera, points, point_count);
		if (!screen_points.empty()) {
			render_segments(screen_points.data(), screen_points.size(), color);
		}
	}

	void prim2d_lines::render_world_segments_looped(const viewport& viewport, const camera& camera, const vec2* points, unsigned int point_count, const color& color) {
		auto screen_points = _world_projection_buffer.project_points(viewport, camera, points, point_count);
		if (!screen_points.empty()) {
			render_segments_looped(screen_points.data(), screen_points.size(), color);
		}
	}

	void prim2d_lines::render_world_segments_looped(const viewport& viewport, const camera& camera, const vec3* points, unsigned int point_count, const color& color) {
		auto screen_points = _world_projection_buffer.project_points(viewport, camera, points, point_count);
		if (!screen_points.empty()) {
			render_segments_looped(screen_points.data(), screen_points.size(), color);
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

}