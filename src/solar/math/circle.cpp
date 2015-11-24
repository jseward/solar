#include "circle.h"

namespace solar {

	circle::circle() 
		: _center()
		, _radius(0.f) {
	}

	circle::circle(const vec2& center, float radius)
		: _center(center)
		, _radius(radius) {
	}

	bool circle::is_position_within(const vec2& p) const {
		return get_distance_squared(_center, p) <= (_radius * _radius);
	}

}