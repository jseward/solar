#include "prim2d_world_projection_buffer.h"

namespace solar {

	prim2d_world_projection_buffer::prim2d_world_projection_buffer() {
	}

	prim2d_world_projection_buffer::~prim2d_world_projection_buffer() {
	}

	const prim2d_world_projection_buffer::screen_point_vector& prim2d_world_projection_buffer::project_points(const viewport& viewport, const camera& camera, const vec2* points, unsigned int point_count) {
		_buffer.clear();
		_buffer.reserve(point_count);
		for (unsigned int i = 0; i < point_count; ++i) {
			vec2 screen_point;
			if (!viewport.try_project(screen_point, camera.get_view_projection_transform(), vec3(points[i]._x, points[i]._y, 0.f))) {
				_buffer.clear();
				break;
			}
			else {
				_buffer.push_back(screen_point);
			}
		}
		return _buffer;
	}

	const prim2d_world_projection_buffer::screen_point_vector& prim2d_world_projection_buffer::project_points(const viewport& viewport, const camera& camera, const vec3* points, unsigned int point_count) {
		_buffer.clear();
		_buffer.reserve(point_count);
		for (unsigned int i = 0; i < point_count; ++i) {
			vec2 screen_point;
			if (!viewport.try_project(screen_point, camera.get_view_projection_transform(), points[i])) {
				_buffer.clear();
				break;
			}
			else {
				_buffer.push_back(screen_point);
			}
		}
		return _buffer;
	}

}
