#include "basis.h"

namespace solar {

	basis::basis()
		: _position()
		, _rotation(make_mat33_identity()) {		
	}

	basis::basis(const vec3& position, const mat33& rotation)
		: _position(position)
		, _rotation(rotation) {
	}

	const vec3& basis::get_position() const {
		return _position;
	}

	const mat33& basis::get_rotation() const {
		return _rotation;
	}

	basis make_camera_basis(const vec3& eye, const vec3& at, const vec3& up) {
		return basis(eye, make_mat33_rotation_with_forward(at - eye, up));
	}
}
