#include "vec2.h"

#include "solar/math/math_helpers.h"
#include "solar/utility/assert.h"

namespace solar {

	vec2::vec2() 
		: _x(0.f)
		, _y(0.f) {
	}

	vec2::vec2(float x, float y)
		: _x(x)
		, _y(y) {
	}

	bool vec2::is_normalized() const {
		return is_approx(get_length(), 1.f, 0.01f);
	}

	float vec2::get_length() const {
		return sqrt(get_length_squared());
	}

	float vec2::get_length_squared() const {
		return (_x * _x) + (_y * _y);
	}

	vec2& vec2::operator+=(const vec2& rhs) {
		_x += rhs._x;
		_y += rhs._y;
		return *this;
	}

	vec2& vec2::operator-=(const vec2& rhs) {
		_x -= rhs._x;
		_y -= rhs._y;
		return *this;
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

	vec2 operator-(const vec2& v) {
		return vec2(-v._x, -v._y);
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

	float get_dot_product(const vec2& v, const vec2& u) {
		return (u._x * v._x) + (u._y * v._y);
	}

	float get_angle_between(const vec2& u, const vec2& v) {
		//u and v must be normalized, but too slow to check
		//ASSERT(u.is_normalized());
		//ASSERT(v.is_normalized());
		const float dot = constrain(-1.f, 1.f, get_dot_product(u, v));
		return acos(dot);
	}

	float get_angle_between_with_direction(const vec2& u, const vec2& v) {
		const float dot = get_dot_product(u, v);
		const float cross = (u._x * v._y) - (u._y * v._x);
		
		//The ATan2 is clockwise negative and counter-clockwise positive. We want the inverse to match DirectX's left handedness so the sign is -1.f
		const float sign = -1.f;
		return atan2(cross, dot) * sign;
	}
}