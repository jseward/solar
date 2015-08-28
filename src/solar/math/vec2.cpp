#include "vec2.h"

#include "solar/math/math_helpers.h"

namespace solar {

	vec2::vec2() 
		: _x(0.f)
		, _y(0.f) {
	}

	vec2::vec2(float x, float y)
		: _x(x)
		, _y(y) {
	}

	float vec2::get_length() const {
		return sqrt(get_length_squared());
	}

	float vec2::get_length_squared() const {
		return (_x * _x) + (_y * _y);
	}

	vec2 operator+(const vec2& lhs, const vec2& rhs) {
		return vec2(lhs._x + rhs._x, lhs._y + rhs._y);
	}

	vec2 operator-(const vec2& lhs, const vec2& rhs) {
		return vec2(lhs._x - rhs._x, lhs._y - rhs._y);
	}

	vec2 operator*(const vec2& lhs, float rhs) {
		return vec2(lhs._x * rhs, lhs._y * rhs);
	}

	float get_distance(const vec2& v0, const vec2& v1) {
		return (v0 - v1).get_length();
	}

}