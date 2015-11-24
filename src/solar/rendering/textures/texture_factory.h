#pragma once

#include "solar/resources/resource_factory_caching_context.h"
#include "texture.h"

namespace solar {

	class texture_factory {
	public:
		virtual void add_texture_pool(const char* texture_pool_name, unsigned int max_size_in_bytes, bool should_create_mip_maps) = 0;
		virtual texture* get_texture(const char* texture_pool_name, const std::string& id, const std::string& id_source_description) = 0;
		virtual const resource_factory_caching_context& get_caching_context() const = 0;
	};

}