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

	mat44 make_mat44_identity() {
		return mat44();
	}

	mat44 make_mat44_perspective_fov(deg fov_y, float aspect_ratio, float near_plane, float far_plane) {
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

		float y_scale = 1.f / tan(fov_y / 2.f);
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

}