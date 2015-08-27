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

	vec2 operator+(const vec2& lhs, const vec2& rhs) {
		return vec2(lhs._x + rhs._x, lhs._y + rhs._y);
	}

	vec2 operator*(const vec2& lhs, float rhs) {
		return vec2(lhs._x * rhs, lhs._y * rhs);
	}

}