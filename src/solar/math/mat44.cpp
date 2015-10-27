#include "mat44.h"

#include "solar/utility/assert.h"
#include "math_helpers.h"

namespace solar {

	mat44::mat44()
		: _v00(1.f), _v01(0.f), _v02(0.f), _v03(0.f)
		, _v10(0.f), _v11(1.f), _v12(0.f), _v13(0.f)
		, _v20(0.f), _v21(0.f), _v22(1.f), _v23(0.f)
		, _v30(0.f), _v31(0.f), _v32(0.f), _v33(1.f) {
	}

	mat44::mat44(
		float v00, float v01, float v02, float v03,
		float v10, float v11, float v12, float v13,
		float v20, float v21, float v22, float v23,
		float v30, float v31, float v32, float v33)
		: _v00(v00), _v01(v01), _v02(v02), _v03(v03)
		, _v10(v10), _v11(v11), _v12(v12), _v13(v13)
		, _v20(v20), _v21(v21), _v22(v22), _v23(v23)
		, _v30(v30), _v31(v31), _v32(v32), _v33(v33) {
	}

	mat44::mat44(const mat33& m) 
		: _v00(m.at(0, 0)), _v01(m.at(0, 1)), _v02(m.at(0, 2)), _v03(0.f)
		, _v10(m.at(1, 0)), _v11(m.at(1, 1)), _v12(m.at(1, 2)), _v13(0.f)
		, _v20(m.at(2, 0)), _v21(m.at(2, 1)), _v22(m.at(2, 2)), _v23(0.f)
		, _v30(0.f), _v31(0.f), _v32(0.f), _v33(1.f) {
	}

	mat44& operator*=(mat44& lhs, const mat44& rhs) {

		mat44 new_lhs = mat44(
			(lhs._v00 * rhs._v00) + (lhs._v01 * rhs._v10) + (lhs._v02 * rhs._v20) + (lhs._v03 * rhs._v30),
			(lhs._v00 * rhs._v01) + (lhs._v01 * rhs._v11) + (lhs._v02 * rhs._v21) + (lhs._v03 * rhs._v31),
			(lhs._v00 * rhs._v02) + (lhs._v01 * rhs._v12) + (lhs._v02 * rhs._v22) + (lhs._v03 * rhs._v32),
			(lhs._v00 * rhs._v03) + (lhs._v01 * rhs._v13) + (lhs._v02 * rhs._v23) + (lhs._v03 * rhs._v33),

			(lhs._v10 * rhs._v00) + (lhs._v11 * rhs._v10) + (lhs._v12 * rhs._v20) + (lhs._v13 * rhs._v30),
			(lhs._v10 * rhs._v01) + (lhs._v11 * rhs._v11) + (lhs._v12 * rhs._v21) + (lhs._v13 * rhs._v31),
			(lhs._v10 * rhs._v02) + (lhs._v11 * rhs._v12) + (lhs._v12 * rhs._v22) + (lhs._v13 * rhs._v32),
			(lhs._v10 * rhs._v03) + (lhs._v11 * rhs._v13) + (lhs._v12 * rhs._v23) + (lhs._v13 * rhs._v33),

			(lhs._v20 * rhs._v00) + (lhs._v21 * rhs._v10) + (lhs._v22 * rhs._v20) + (lhs._v23 * rhs._v30),
			(lhs._v20 * rhs._v01) + (lhs._v21 * rhs._v11) + (lhs._v22 * rhs._v21) + (lhs._v23 * rhs._v31),
			(lhs._v20 * rhs._v02) + (lhs._v21 * rhs._v12) + (lhs._v22 * rhs._v22) + (lhs._v23 * rhs._v32),
			(lhs._v20 * rhs._v03) + (lhs._v21 * rhs._v13) + (lhs._v22 * rhs._v23) + (lhs._v23 * rhs._v33),

			(lhs._v30 * rhs._v00) + (lhs._v31 * rhs._v10) + (lhs._v32 * rhs._v20) + (lhs._v33 * rhs._v30),
			(lhs._v30 * rhs._v01) + (lhs._v31 * rhs._v11) + (lhs._v32 * rhs._v21) + (lhs._v33 * rhs._v31),
			(lhs._v30 * rhs._v02) + (lhs._v31 * rhs._v12) + (lhs._v32 * rhs._v22) + (lhs._v33 * rhs._v32),
			(lhs._v30 * rhs._v03) + (lhs._v31 * rhs._v13) + (lhs._v32 * rhs._v23) + (lhs._v33 * rhs._v33));

		lhs = new_lhs;
		return lhs;
	}

	mat44 operator*(const mat44& lhs, const mat44& rhs) {
		mat44 out = lhs;
		return (out *= rhs);
	}

	vec2 operator*(const vec2& vec, const mat44& mat) {
		return vec2(
			(vec._x * mat._v00) + (vec._y * mat._v10) + mat._v30,
			(vec._x * mat._v01) + (vec._y * mat._v11) + mat._v31);
	}

	vec3 operator*(const vec3& vec, const mat44& mat) {
		return vec3(
			(vec._x * mat._v00) + (vec._y * mat._v10) + (vec._z * mat._v20) + mat._v30,
			(vec._x * mat._v01) + (vec._y * mat._v11) + (vec._z * mat._v21) + mat._v31,
			(vec._x * mat._v02) + (vec._y * mat._v12) + (vec._z * mat._v22) + mat._v32);
	}

	vec4 operator*(const vec4& vec, const mat44& mat) {
		return vec4(
			(vec._x * mat._v00) + (vec._y * mat._v10) + (vec._z * mat._v20) + (vec._w * mat._v30),
			(vec._x * mat._v01) + (vec._y * mat._v11) + (vec._z * mat._v21) + (vec._w * mat._v31),
			(vec._x * mat._v02) + (vec._y * mat._v12) + (vec._z * mat._v22) + (vec._w * mat._v32),
			(vec._x * mat._v03) + (vec._y * mat._v13) + (vec._z * mat._v23) + (vec._w * mat._v33));
	}

	mat44& operator/=(mat44& mat, float k) {
		ASSERT(k != 0.f);
		mat._v00 /= k; mat._v01 /= k; mat._v02 /= k; mat._v03 /= k;
		mat._v10 /= k; mat._v11 /= k; mat._v12 /= k; mat._v13 /= k;
		mat._v20 /= k; mat._v21 /= k; mat._v22 /= k; mat._v23 /= k;
		mat._v30 /= k; mat._v31 /= k; mat._v32 /= k; mat._v33 /= k;
		return mat;
	}

	mat44 make_mat44_identity() {
		return mat44();
	}

	mat44 make_mat44_perspective_fov(float fov_y_radians, float aspect_ratio, float near_plane, float far_plane) {
		//Direct3D docs: D3DXMatrixPerspectiveFovLH
		//
		//x_scale    0          0               0
		//0        y_scale      0               0
		//0          0       zf/(zf-zn)         1
		//0          0       -zn*zf/(zf-zn)     0
		//
		//y_scale = cot(fovY/2)
		//x_scale = y_scale / aspect ratio

		ASSERT(aspect_ratio > 0.f);
		ASSERT(near_plane < far_plane);

		float y_scale = 1.f / tan(fov_y_radians / 2.f);
		float x_scale = y_scale / aspect_ratio;
		float q = far_plane / (far_plane - near_plane);

		return mat44(
			x_scale, 0.f, 0.f, 0.f,
			0.f, y_scale, 0.f, 0.f,
			0.f, 0.f, q, 1.f,
			0.f, 0.f, (-near_plane * q), 0.f);
	}

	mat44 make_mat44_orthographic(float width, float height, float near_plane, float far_plane) {
		//Direct3D docs: D3DXMatrixOrthoLH
		//
		//2/w  0    0           0
		//0    2/h  0           0
		//0    0    1/(zf-zn)   0
		//0    0   -zn/(zf-zn)  1

		ASSERT(near_plane < far_plane);
		float q = 1.f / (far_plane - near_plane);

		return mat44(
			2.f / width, 0.f, 0.f, 0.f,
			0.f, 2.f / height, 0.f, 0.f,
			0.f, 0.f, q, 0.f,
			0.f, 0.f, (-near_plane * q), 1.f);
	}

	mat44 make_mat44_camera_view(const vec3& position, const mat33& rotation) {
		mat44 m;
		for (int i = 0; i < 3; i++) {
			m.at(3, i) = -dot(position, rotation.get_row(i));
			m.at(i, 0) = rotation.at(0, i);
			m.at(i, 1) = rotation.at(1, i);
			m.at(i, 2) = rotation.at(2, i);
			m.at(i, 3) = 0.f;
		}
		m.at(3, 3) = 1.f;
		return m;
	}

	mat44 make_mat44_inverted(const mat44& in) {
		//http://www.gamedev.net/community/forums/topic.asp?topic_id=376432
		const float a = in._v00;
		const float b = in._v01;
		const float c = in._v02;
		const float d = in._v03;
		const float e = in._v10;
		const float f = in._v11;
		const float g = in._v12;
		const float h = in._v13;
		const float i = in._v20;
		const float j = in._v21;
		const float k = in._v22;
		const float l = in._v23;
		const float m = in._v30;
		const float n = in._v31;
		const float o = in._v32;
		const float p = in._v33;

		mat44 out = mat44(
			(-h * k * n) + (g * l * n) + (h * j * o) - (f * l * o) - (g * j * p) + (f * k * p),
			(d * k * n) - (c * l * n) - (d * j * o) + (b * l * o) + (c * j * p) - (b * k * p),
			(-d * g * n) + (c * h * n) + (d * f * o) - (b * h * o) - (c * f * p) + (b * g * p),
			(d * g * j) - (c * h * j) - (d * f * k) + (b * h * k) + (c * f * l) - (b * g * l),

			(h * k * m) - (g * l * m) - (h * i * o) + (e * l * o) + (g * i * p) - (e * k * p),
			(-d * k * m) + (c * l * m) + (d * i * o) - (a * l * o) - (c * i * p) + (a * k * p),
			(d * g * m) - (c * h * m) - (d * e * o) + (a * h * o) + (c * e * p) - (a * g * p),
			(-d * g * i) + (c * h * i) + (d * e * k) - (a * h * k) - (c * e * l) + (a * g * l),

			(-h * j * m) + (f * l * m) + (h * i * n) - (e * l * n) - (f * i * p) + (e * j * p),
			(d * j * m) - (b * l * m) - (d * i * n) + (a * l * n) + (b * i * p) - (a * j * p),
			(-d * f * m) + (b * h * m) + (d * e * n) - (a * h * n) - (b * e * p) + (a * f * p),
			(d * f * i) - (b * h * i) - (d * e * j) + (a * h * j) + (b * e * l) - (a * f * l),

			(g * j * m) - (f * k * m) - (g * i * n) + (e * k * n) + (f * i * o) - (e * j * o),
			(-c * j * m) + (b * k * m) + (c * i * n) - (a * k * n) - (b * i * o) + (a * j * o),
			(c * f * m) - (b * g * m) - (c * e * n) + (a * g * n) + (b * e * o) - (a * f * o),
			(-c * f * i) + (b * g * i) + (c * e * j) - (a * g * j) - (b * e * k) + (a * f * k));

		const float det =
			(b * h * k * m) - (b * g * l * m) - (a * h * k * n) + (a * g * l * n) - (b * h * i * o) + (a * h * j * o) + (b * e * l * o) - (a * f * l * o) +
			(d * ((g * j * m) - (f * k * m) - (g * i * n) + (e * k * n) + (f * i * o) - (e * j * o))) +
			(p * ((b * g * i) - (a * g * j) - (b * e * k) + (a * f * k))) +
			(c * ((-h * j * m) + (f * l * m) + (h * i * n) - (e * l * n) - (f * i * p) + (e * j * p)));
		ASSERT(det != 0.f);
		out /= det;

		return out;
	}

	mat44 make_mat44_rotation_on_x(float radians) {
		return mat44(make_mat33_rotation_on_x(radians));
	}

	mat44 make_mat44_rotation_on_y(float radians) {
		return mat44(make_mat33_rotation_on_y(radians));
	}

	mat44 make_mat44_rotation_on_z(float radians) {
		return mat44(make_mat33_rotation_on_z(radians));
	}

	mat44 make_mat44_translation(const vec3& t) {
		return make_mat44_translation(t._x, t._y, t._z);
	}

	mat44 make_mat44_translation(float x, float y, float z) {
		mat44 m = make_mat44_identity();
		m._v30 = x;
		m._v31 = y;
		m._v32 = z;
		return m;
	}

}