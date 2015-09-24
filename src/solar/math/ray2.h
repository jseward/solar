#pragma once

#include "vec2.h"

namespace solar {

	class ray2 {
	private:
		vec2 _origin;
		vec2 _dir_with_magnitude;

	public:
		ray2();

		vec2 get_position_on_ray(float t) const;

		float get_closest_t_unconstrained(const vec2& p) const;
		float get_closest_t_constrained(const vec2& p) const;

	public:
		friend ray2 make_ray2_begin_end(const vec2& begin, const vec2& end);
	};

}