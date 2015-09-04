#pragma once

#include <vector>
#include "solar/math/vec2.h"
#include "solar/rendering/viewport.h"
#include "solar/rendering/cameras/camera.h"

namespace solar {

	class prim2d_world_projection_buffer {
	public:
		using screen_point_vector = std::vector<vec2>;

	private:
		screen_point_vector _buffer;

	public:
		prim2d_world_projection_buffer();
		~prim2d_world_projection_buffer();

		const screen_point_vector& project_points(const viewport& viewport, const camera& camera, const vec2* points, unsigned int point_count);
		const screen_point_vector& project_points(const viewport& viewport, const camera& camera, const vec3* points, unsigned int point_count);
	};

}