#include "opengl_mesh.h"

namespace solar {

	opengl_mesh::opengl_mesh(const resource_address& resource_address)
		: _resource_address(resource_address) {
	}

	opengl_mesh::~opengl_mesh() {
	}

	const resource_address& opengl_mesh::get_resource_address() const {
		return _resource_address;
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