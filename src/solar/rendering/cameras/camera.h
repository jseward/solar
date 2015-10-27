#pragma once

#include "solar/math/basis.h"
#include "solar/math/mat44.h"
#include "solar/rendering/viewport.h"
#include "camera_projection_type.h"
#include "camera_cached_value.h"
#include "camera_frustrum.h"

namespace solar {

	class camera {
	private:
		camera_projection_type _projection_type;
		float _fov_y_radians;
		float _aspect_ratio;
		float _ortho_width;
		float _ortho_height;
		float _near_plane;
		float _far_plane;
		basis _basis;

		mutable int _cache_context;
		mutable camera_cached_value<camera_frustrum> _frustrum;
		mutable camera_cached_value<mat44> _view_transform;
		mutable camera_cached_value<mat44> _projection_transform;
		mutable camera_cached_value<mat44> _view_projection_transform;

	public:
		camera();

		camera& set_projection_type(camera_projection_type type);
		camera& set_aspect_ratio(float aspect_ratio);
		camera& set_ortho_width_height(float width, float height);
		camera& set_near_far_planes(float near_plane, float far_plane);
		camera& set_basis(const basis& basis);

		const basis& get_basis() const;
		const mat44& get_view_transform() const;
		const mat44& get_projection_transform() const;
		const mat44& get_view_projection_transform() const;

		mat44 build_world_view_projection_transform(const mat44& world_transform) const;

	private:
		void invalidate_cache();
	};

}