#include "mat33.h"

#include "math_helpers.h"

namespace solar {

	const int mat33::FORWARD_ROW_INDEX = 2;
	const int mat33::UP_ROW_INDEX = 1;
	const int mat33::CROSS_ROW_INDEX = 0;

	mat33::mat33()
		: _v00(1.f), _v01(0.f), _v02(0.f)
		, _v10(0.f), _v11(1.f), _v12(0.f)
		, _v20(0.f), _v21(0.f), _v22(1.f) {
	}

	mat33::mat33(
		float v00, float v01, float v02,
		float v10, float v11, float v12,
		float v20, float v21, float v22)
		: _v00(v00), _v01(v01), _v02(v02)
		, _v10(v10), _v11(v11), _v12(v12)
		, _v20(v20), _v21(v21), _v22(v22) {
	}

	vec2 operator*(const vec2& vec, const mat33& mat) {
		return vec2(
			(vec._x * mat._v00) + (vec._y * mat._v10),
			(vec._x * mat._v01) + (vec._y * mat._v11));
	}

	vec3 operator*(const vec3& vec, const mat33& mat) {
		return vec3(
			(vec._x * mat._v00) + (vec._y * mat._v10) + (vec._z * mat._v20),
			(vec._x * mat._v01) + (vec._y * mat._v11) + (vec._z * mat._v21),
			(vec._x * mat._v02) + (vec._y * mat._v12) + (vec._z * mat._v22));
	}

	mat33 make_mat33_identity() {
		return mat33(); //default constructor = identity, this function makes intent of clients clearer though.
	}

	mat33 make_mat33_rotation_on_x(float radians) {
		float c = cos(radians);
		float s = sin(radians);
		return mat33(
			1.f, 0.f, 0.f,
			0.f, c, s,
			0.f, -s, c);
	}

	mat33 make_mat33_rotation_on_y(float radians) {
		float c = cos(radians);
		float s = sin(radians);
		return mat33(
			c, 0.f, -s,
			0.f, 1.f, 0.f,
			s, 0.f, c);
	}

	mat33 make_mat33_rotation_on_z(float radians) {
		float c = cos(radians);
		float s = sin(radians);
		return mat33(
			c, s, 0.f,
			-s, c, 0.f,
			0.f, 0.f, 1.f);
	}

	mat33 make_mat33_rotation_with_forward(const vec3& forward, const vec3& up) {
		mat33 out;
		vec3& out_forward = out.get_row(mat33::FORWARD_ROW_INDEX);
		vec3& out_up = out.get_row(mat33::UP_ROW_INDEX);
		vec3& out_cross = out.get_row(mat33::CROSS_ROW_INDEX);

		out_forward = normalize(forward);
		out_up = normalize(up);
		if (are_collinear(out_forward, out_up)) {
			vec3 bad_up = out_up;
			out_up = vec3(-bad_up._y, bad_up._x, bad_up._z);
		}
		out_cross = normalize(cross(out_up, out_forward));
		out_up = cross(out_forward, out_cross);

		return out;
	}
}