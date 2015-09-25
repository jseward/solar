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

	vec2 operator*(const vec2& lhs, float k) {
		return vec2(lhs._x * k, lhs._y * k);
	}

	vec2 operator/(const vec2& lhs, float k) {
		ASSERT(k != 0.f);
		return vec2(lhs._x / k, lhs._y / k);
	}

	vec2 make_perpendicular(const vec2& in, float dir) {
		ASSERT(dir == 1.f || dir == -1.f);
		return vec2(dir * in._y, -dir * in._x);
	}

	vec2 normalize(const vec2& in) {
		float length = in.get_length();
		if (!is_approx(length, 0.f, 0.00001f)) {
			return in / length;
		}
		return vec2(1.f, 0.f);
	}

	float get_distance(const vec2& v0, const vec2& v1) {
		return (v0 - v1).get_length();
	}

	float get_distance_squared(const vec2& v0, const vec2& v1) {
		return (v0 - v1).get_length_squared();
	}

}