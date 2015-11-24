#pragma once

#include "vec3.h"

namespace solar {

	class mat33 {
	public:
		static const int FORWARD_ROW_INDEX;
		static const int UP_ROW_INDEX;
		static const int CROSS_ROW_INDEX;

	private:
		#pragma warning(push)
		#pragma warning(disable:4201) //warning C4201: nonstandard extension used: nameless struct/union
		union {
			float _values[3][3];			
			struct {
				float _v00, _v01, _v02;
				float _v10, _v11, _v12;
				float _v20, _v21, _v22;
			};
		};
		#pragma warning(pop)

	public:
		mat33();
		mat33(
			float v00, float v01, float v02,
			float v10, float v11, float v12,
			float v20, float v21, float v22);
		
		float& at(int row, int column);
		float at(int row, int column) const;
		const vec3& get_row(int row) const;
		vec3& get_row(int row);
		const vec3& get_forward() const;
		const vec3& get_up() const;
		const vec3& get_cross() const;

	public:
		friend vec2 operator*(const vec2& vec, const mat33& mat);
		friend vec3 operator*(const vec3& vec, const mat33& mat);

	public:
		friend mat33 make_mat33_identity();
		friend mat33 make_mat33_rotation_on_x(float radians);
		friend mat33 make_mat33_rotation_on_y(float radians);
		friend mat33 make_mat33_rotation_on_z(float radians);
		friend mat33 make_mat33_rotation_with_forward(const vec3& forward, const vec3& up);
	};

	inline float& mat33::at(int row, int column) {
		return _values[row][column];
	}

	inline float mat33::at(int row, int column) const {
		return _values[row][column];
	}

	inline const vec3& mat33::get_row(int row) const {
		return reinterpret_cast<const vec3&>(_values[row][0]);
	}

	inline vec3& mat33::get_row(int row) {
		return reinterpret_cast<vec3&>(_values[row][0]);
	}

	inline const vec3& mat33::get_forward() const {
		return get_row(FORWARD_ROW_INDEX);
	}

	inline const vec3& mat33::get_up() const {
		return get_row(UP_ROW_INDEX);
	}

	inline const vec3& mat33::get_cross() const {
		return get_row(CROSS_ROW_INDEX);
	}

}