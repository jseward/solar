#pragma once

#include "vec3.h"
#include "mat33.h"

namespace solar {

	class basis {
	private:
		vec3 _position;
		mat33 _rotation;

	public:
		basis();
		basis(const vec3& position, const mat33& rotation);

		const vec3& get_position() const;
		const mat33& get_rotation() const;

	private:
		friend basis make_camera_basis(const vec3& eye, const vec3& at, const vec3& up);
	};

}
