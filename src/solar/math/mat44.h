#pragma once

#include "vec2.h"
#include "mat33.h"

namespace solar {

	class mat44 {
	private:
		#pragma warning(push)
		#pragma warning(disable:4201) //warning C4201: nonstandard extension used: nameless struct/union
		union {
			float _values[4][4];			
			struct {
				float _v00, _v01, _v02, _v03;
				float _v10, _v11, _v12, _v13;
				float _v20, _v21, _v22, _v23;
				float _v30, _v31, _v32, _v33;
			};
		};
		#pragma warning(pop)

	public:
		mat44();
		mat44(
			float v00, float v01, float v02, float v03,
			float v10, float v11, float v12, float v13,
			float v20, float v21, float v22, float v23,
			float v30, float v31, float v32, float v33);

		float& at(int row, int column);
		float at(int row, int column) const;

	public:
		friend mat44& operator*=(mat44& lhs, const mat44& rhs);
		friend mat44 operator*(const mat44& lhs, const mat44& rhs);

	public:
		friend mat44 make_mat44_identity();
		friend mat44 make_mat44_camera_view(const vec3& position, const mat33& rotation);
		friend mat44 make_mat44_perspective_fov(deg fov_y, float aspect_ratio, float near_plane, float far_plane);
		friend mat44 make_mat44_orthographic(float width, float height, float near_plane, float far_plane);
	};

	inline float& mat44::at(int row, int column) {
		return _values[row][column];
	}

	inline float mat44::at(int row, int column) const {
		return _values[row][column];
	}

}