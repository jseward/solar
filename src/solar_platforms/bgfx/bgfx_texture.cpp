#include "bgfx_texture.h"

namespace solar {

	bgfx_texture::bgfx_texture(const resource_address& resource_address)
		: _resource_address(resource_address) {
	}

	bgfx_texture::~bgfx_texture() {
	}

	size bgfx_texture::request_size() {
		//todo
		return size();
	}

}