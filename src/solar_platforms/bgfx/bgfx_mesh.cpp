#include "bgfx_mesh.h"

namespace solar {

	bgfx_mesh::bgfx_mesh(const mesh_texture_params& texture_params, const resource_address& resource_address)
		: _texture_params(texture_params)
		, _resource_address(resource_address) {
	}

	bgfx_mesh::~bgfx_mesh() {
	}

	const resource_address& bgfx_mesh::get_resource_address() const {
		return _resource_address;
	}

	const mesh_texture_params& bgfx_mesh::get_texture_params() const {
		return _texture_params;
	}

	void bgfx_mesh::create_rendering_resources() {
		//todo
	}

	void bgfx_mesh::release_rendering_resources() {
		//todo
	}

	int bgfx_mesh::get_sub_mesh_count() const {
		//todo
		return 0;
	}

}