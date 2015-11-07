#pragma once

#include "solar/rendering/textures/texture.h"
#include "solar/resources/resource_address.h"

namespace solar {

	class bgfx_texture : public texture {
	private:
		resource_address _resource_address;

	public:
		bgfx_texture(const resource_address& resource_address);
		virtual ~bgfx_texture();

		virtual size request_size() override;
	};

}