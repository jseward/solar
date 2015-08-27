#include "viewport.h"

#include "solar/utility/type_convert.h"
#include "solar/math/math_helpers.h"

namespace solar {

	viewport::viewport()
		: _x(0)
		, _y(0)
		, _width(0)
		, _height(0)
		, _min_z(0.f)
		, _max_z(1.f) {
	}

	float viewport::get_aspect_ratio() const {
		if (_height > 0) {
			return uint_to_float(_width) / uint_to_float(_height);
		}
		return 0.f;
	}

	viewport& viewport::set_area(const rect& area) {
		_x = area.get_left();
		_y = area.get_top();
		_width = area.get_width();
		_height = area.get_height();
		return *this;
	}

	viewport& viewport::set_min_max_z(float min_z, float max_z) {
		_min_z = min_z;
		_max_z = max_z;
		return *this;
	}

	bool viewport::try_project(vec2& screen_position, const mat44& view_projection_transform, const vec3& world_position) const {
		vec3 sp3;
		if (try_project(sp3, view_projection_transform, world_position)) {
			screen_position._x = sp3._x;
			screen_position._y = sp3._y;
			return true;
		}
		return false;
	}

	bool viewport::try_project(vec3& screen_position, const mat44& view_projection_transform, const vec3& world_position) const {
		//This algorithm differs slightly from D3DXVec3Project() in that it can fail if w comes out 0. D3DX just returns the 
		//center of the viewport in that situation.
		auto projected_4 = vec4(world_position._x, world_position._y, world_position._z, 1.f) * view_projection_transform;
		if (std::abs(projected_4._w) < 0.1f) {
			return false;
		}

		auto projected_3 = vec3(projected_4._x, projected_4._y, projected_4._z) / projected_4._w;

		screen_position._x = _x + (((1.f + projected_3._x) / 2.f) * _width);
		screen_position._y = _y + (((1.f - projected_3._y) / 2.f) * _height);
		screen_position._z = projected_3._z;

		//NOTE: the math to convert to screen space breaks down as the position approaches the center of the screen. Due to floating point errors
		//slight changes in either the camera position will produce different screen space values. This will cause jittered and looks very bad.
		//To remove the jittering, snap the screen position to the center of the viewport if it is close. This is hacky but is cheap and works.
		const float jitter_tolerance = 2.f;
		const float center_x = _x + (_width / 2.f);
		const float center_y = _y + (_height / 2.f);
		if (is_approx(screen_position._x, center_x, jitter_tolerance) && is_approx(screen_position._y, center_y, jitter_tolerance)) {
			screen_position._x = center_x;
			screen_position._y = center_y;
		}

		return true;
	}

}