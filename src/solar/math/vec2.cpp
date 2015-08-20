#include "vec2.h"

namespace solar {

	vec2::vec2() 
		: _x(0.f)
		, _y(0.f) {
	}

	vec2::vec2(float x, float y)
		: _x(x)
		, _y(y) {
	}

	float vec2::get_x() const {
		return _x;
	}

	float vec2::get_y() const {
		return _y;
	}

}