#include "bgfx_texture_factory.h"

#include "solar/utility/assert.h"
#include "solar/utility/unused_parameter.h"
#include "solar/resources/resource_system.h"
#include "bgfx_texture.h"

namespace solar {

	bgfx_texture_factory::bgfx_texture_factory(resource_system& resource_system)
		: _resource_system(resource_system)
		, _is_setup(false) {
	}

	bgfx_texture_factory::~bgfx_texture_factory() {
		ASSERT(!_is_setup);
		ASSERT(_textures.empty());
	}

	void bgfx_texture_factory::setup() {
		ASSERT(!_is_setup);
		_is_setup = true;
	}

	void bgfx_texture_factory::teardown() {
		remove_all_textures();
		_is_setup = false;
	}

	void bgfx_texture_factory::remove_all_textures() {
		_textures.clear();
		_caching_context.increment();
	}

	void bgfx_texture_factory::add_texture_pool(const char* texture_pool_name, unsigned int max_size_in_bytes, bool should_create_mip_maps) {
		//todo - handle texture pools
		UNUSED_PARAMETER(texture_pool_name);
		UNUSED_PARAMETER(max_size_in_bytes);
		UNUSED_PARAMETER(should_create_mip_maps);
	}

	texture* bgfx_texture_factory::get_texture(const char* texture_pool_name, const std::string& id, const std::string& id_source_description) {
		ASSERT(_is_setup);

		auto iter = _textures.find(id);
		if (iter != _textures.end()) {
			//todo - handle texture pools
			//if (iter->second->get_texture_pool().get_name() != texture_pool_name) {
			//	ALERT("bgfx_texture cannot be used in different texture_pools.\n\ntexture_id: '{}'\nrequested_pool_name: '{}'\ncached_pool_name: '{}'", id, texture_pool_name, iter->second->get_texture_pool().get_name());
			//}
			return iter->second.get();
		}

		//todo - handle texture pools
		UNUSED_PARAMETER(texture_pool_name);
		//bgfx_texture_pool* texture_pool = nullptr;
		//auto texture_pool_iter = _texture_pools.find(texture_pool_name);
		//if (texture_pool_iter == _texture_pools.end()) {
		//	ALERT("bgfx_texture requested a texture_pool that does not exist.\n\ntexture_id: '{}'\nrequested_pool_name: '{}'", id, texture_pool_name);
		//	texture_pool = _null_texture_pool.get();
		//}
		//else {
		//	texture_pool = texture_pool_iter->second.get();
		//}
		
		auto address = _resource_system.resolve_address("texture", "textures", ".tga;.dds", id.c_str(), id_source_description.c_str());
		bgfx_texture* new_texture = new bgfx_texture(address);
		_textures[id] = std::unique_ptr<bgfx_texture>(new_texture);

		return new_texture;
	}

	const resource_factory_caching_context& bgfx_texture_factory::get_caching_context() const {
		return _caching_context;
	}

}
