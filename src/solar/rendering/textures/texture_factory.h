#pragma once

#include "solar/resources/resource_address.h"

#include "texture.h"
#include "texture_create_params.h"

namespace solar {

	class texture_factory {
	public:
		virtual texture* create_texture(const resource_address& address, const texture_create_params& params) = 0;
		virtual void release_texture(texture* texture) = 0;
	};

}