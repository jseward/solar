#include "vec3.h"

#include "solar/math/math_helpers.h"

namespace solar {

	vec3::vec3()
		: _x(0.f)
		, _y(0.f)
		, _z(0.f) {
	}

	vec3::vec3(float x, float y, float z)
		: _x(x)
		, _y(y)
		, _z(z) {
	}

	bool vec3::operator==(const vec3& rhs) const {
		return
			_x == rhs._x &&
			_y == rhs._y &&
			_z == rhs._z;
	}

	bool vec3::operator!=(const vec3& rhs) const {
		return !(*this == rhs);
	}

	float vec3::get_length() const {
		return sqrt(get_length_squared());
	}

	float vec3::get_length_squared() const {
		return (_x * _x) + (_y * _y) + (_z * _z);
	}

	vec3 operator/(const vec3& lhs, float rhs) {
		return vec3(lhs._x / rhs, lhs._y / rhs, lhs._z / rhs);
	}

	vec3 operator-(const vec3& lhs, const vec3& rhs) {
		return vec3(lhs._x - rhs._x, lhs._y - rhs._y, lhs._z - rhs._z);
	}

	vec3 normalize(const vec3& in) {
		float length = in.get_length();
		if (!is_approx(length, 0.f, 0.00001f)) {
			return in / length;
		}
		return vec3(1.f, 0.f, 0.f);
	}

	float dot(const vec3& u, const vec3& v) {
		return (u._x * v._x) + (u._y * v._y) + (u._z * v._z);
	}

	vec3 cross(const vec3& u, const vec3& v) {
		return vec3(
			(u._y * v._z) - (u._z * v._y),
			(u._z * v._x) - (u._x * v._z),
			(u._x * v._y) - (u._y * v._x));
	}

	bool are_collinear(const vec3& u, const vec3& v) {
		float d = abs(constrain(-1.f, +1.f, dot(u, v)));
		return is_approx(d, 1.f, 0.001f);
	}

}