#pragma once

#include "solar/math/rect.h"
#include "solar/math/mat44.h"

namespace solar {

	class viewport {
	public:
		unsigned int _x;
		unsigned int _y;
		unsigned int _width;
		unsigned int _height;
		float _min_z;
		float _max_z;

	public:
		viewport();

		rect get_area() const;
		float get_aspect_ratio() const;

		viewport& set_area(const rect& area);
		viewport& set_min_max_z(float min_z, float max_z);

		bool try_project(vec2& screen_position, const mat44& view_projection_transform, const vec3& world_position) const;
		bool try_project(vec3& screen_position, const mat44& view_projection_transform, const vec3& world_position) const;

		bool try_unproject(vec3& world_position, const mat44& view_projection_transform, const point& screen_position) const;
	};

}