#include "ray2.h"

#include "math_helpers.h"

namespace solar {

	ray2::ray2() {
	}

	ray2 make_ray2_begin_end(const vec2& begin, const vec2& end) {
		ray2 r;
		r._origin = begin;
		r._dir_with_magnitude = (end - begin);
		return r;
	}

	vec2 ray2::get_position_on_ray(float t) const {
		return _origin + (_dir_with_magnitude * t);
	}

	float ray2::get_closest_t_unconstrained(const vec2& p) const {
		float t = 0.f;
		float ray_length_squared = _dir_with_magnitude.get_length_squared();
		if (ray_length_squared > 0.0f) {
			t = (
				((p._x - _origin._x) * _dir_with_magnitude._x) +
				((p._y - _origin._y) * _dir_with_magnitude._y)) / ray_length_squared;
		}
		return t;
	}

	float ray2::get_closest_t_constrained(const vec2& p) const {
		return constrain(0.f, 1.f, get_closest_t_unconstrained(p));
	}

}