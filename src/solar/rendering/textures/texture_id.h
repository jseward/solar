#pragma once

#include <string>
#include "solar/resources/resource_id.h"
#include "texture.h"
#include "texture_factory.h"

namespace solar {

	class texture_id : public resource_id<texture, texture_factory> {
	private:
		const char* _texture_pool_name;

	public:
		texture_id(const char* texture_pool_name);
		virtual int get_current_resource_caching_context(const texture_factory& factory) const override;
		virtual texture* get_uncached_resource(texture_factory& factory, const std::string& id, const std::string& id_source_description) const override;
	};

}