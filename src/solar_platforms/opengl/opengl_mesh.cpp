#include "opengl_mesh.h"

namespace solar {

	opengl_mesh::opengl_mesh(const mesh_texture_params& texture_params, const resource_address& resource_address)
		: _texture_params(texture_params)
		, _resource_address(resource_address) {
	}

	opengl_mesh::~opengl_mesh() {
	}

	const resource_address& opengl_mesh::get_resource_address() const {
		return _resource_address;
	}

	const mesh_texture_params& opengl_mesh::get_texture_params() const {
		return _texture_params;
	}

	void opengl_mesh::create_rendering_resources() {
		//todo
	}

	void opengl_mesh::release_rendering_resources() {
		//todo
	}

	int opengl_mesh::get_sub_mesh_count() const {
		//todo
		return 0;
	}

}