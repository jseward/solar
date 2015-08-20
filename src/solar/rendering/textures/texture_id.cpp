#include "texture_id.h"

namespace solar {

	texture_factory* texture_id::_s_factory = nullptr;

	texture_id::texture_id(const char* texture_pool_name) 
		: _texture_pool_name(texture_pool_name) {
	}

	int texture_id::get_current_resource_caching_context(const texture_factory& factory) const {
		return factory.get_texture_caching_context();
	}

	texture* texture_id::get_uncached_resource(texture_factory& factory, const std::string& id, const std::string& id_source_description) const {
		return factory.get_texture(_texture_pool_name, id, id_source_description);
	}

}