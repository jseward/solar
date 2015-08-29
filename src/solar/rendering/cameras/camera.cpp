#include "camera.h"

#include "solar/math/math_constants.h"
#include "solar/utility/assert.h"
#include "solar/utility/type_convert.h"

namespace solar {

	camera::camera() 
		: _projection_type(camera_projection_type::PERSPECTIVE)
		, _fov_y(45.f)
		, _aspect_ratio(1.f)
		, _ortho_width(0.f)
		, _ortho_height(0.f)
		, _near_plane(0.1f)
		, _far_plane(1000.f)
		, _basis()

		, _cache_context(0) {
	}

	camera& camera::set_projection_type(camera_projection_type type) {
		_projection_type = type;
		invalidate_cache();
		return *this;
	}

	camera& camera::set_aspect_ratio(float aspect_ratio) {
		ASSERT(_projection_type == camera_projection_type::PERSPECTIVE);
		ASSERT(aspect_ratio > 0.f);
		_aspect_ratio = aspect_ratio;
		invalidate_cache();
		return *this;
	}

	camera& camera::set_ortho_width_height(float width, float height) {
		ASSERT(_projection_type == camera_projection_type::ORTHOGRAPHIC);
		ASSERT(width > 0.f);
		ASSERT(height > 0.f);
		_ortho_width = width;
		_ortho_height = height;
		invalidate_cache();
		return *this;
	}

	camera& camera::set_near_far_planes(float near_plane, float far_plane) {
		ASSERT(near_plane < far_plane);
		ASSERT(far_plane > 0.f);
		_near_plane = near_plane;
		_far_plane = far_plane;
		invalidate_cache();
		return *this;
	}

	camera& camera::set_basis(const basis& basis) {
		_basis = basis;
		invalidate_cache();
		return *this;
	}

	const basis& camera::get_basis() const {
		return _basis;
	}

	const mat44& camera::get_view_transform() const {
		if (!_view_transform.is_cached(_cache_context)) {
			_view_transform.set(_cache_context, make_mat44_camera_view(_basis.get_position(), _basis.get_rotation()));
		}
		return _view_transform.get(_cache_context);
	}

	const mat44& camera::get_projection_transform() const {
		if (!_projection_transform.is_cached(_cache_context)) {
			if (_projection_type == camera_projection_type::PERSPECTIVE) {
				_projection_transform.set(_cache_context, make_mat44_perspective_fov(_fov_y, _aspect_ratio, _near_plane, _far_plane));
			}
			else if (_projection_type == camera_projection_type::ORTHOGRAPHIC) {
				_projection_transform.set(_cache_context, make_mat44_orthographic(_ortho_width, _ortho_height, _near_plane, _far_plane));
			}
			else {
				ASSERT(false);
			}
		}
		return _projection_transform.get(_cache_context);
	}

	const mat44& camera::get_view_projection_transform() const {
		if (!_view_projection_transform.is_cached(_cache_context)) {
			_view_projection_transform.set(_cache_context, get_view_transform() * get_projection_transform());
		}
		return _view_projection_transform.get(_cache_context);
	}

	mat44 camera::build_world_view_projection_transform(const mat44& world_transform) const {
		return world_transform * get_view_projection_transform();
	}

	void camera::invalidate_cache() {
		_cache_context++;
	}
}