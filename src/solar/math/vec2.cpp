#include "vec2.h"

#include "solar/math/math_helpers.h"
#include "solar/math/math_constants.h"
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

	float cross(const vec2& u, const vec2& v) {
		return (u._x * v._y) - (u._y * v._x);
	}

	float dot(const vec2& u, const vec2& v) {
		return (u._x * v._x) + (u._y * v._y);
	}

	float get_angle_between_using_any_dir(const vec2& u, const vec2& v) {
		//u and v must be normalized, but too slow to check
		//ASSERT(u.is_normalized());
		//ASSERT(v.is_normalized());
		return acos(constrain(-1.f, 1.f, dot(u, v)));
	}

	float get_angle_between_using_cc_only(const vec2& u, const vec2& v) {
		float a = atan2(cross(u, v), dot(u, v));
		//will be in the range [-PI , PI]
		//we want [0 , TWO_PI]
		if (a < 0.f) {
			return a + TWO_PI;
		}
		return a;
	}

	bool is_approx(const vec2& v0, const vec2& v1, float tolerance) {
		return
			is_approx(v0._x, v1._x, tolerance) &&
			is_approx(v0._y, v1._y, tolerance);
	}

	std::ostream& operator<<(std::ostream& os, const vec2& v) {
		return os << "{ " << v._x << ", " << v._y << " }";
	}

}