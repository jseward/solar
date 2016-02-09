#include "opengl_texture_factory.h"

#include "solar/utility/assert.h"
#include "solar/utility/unused_parameter.h"
#include "solar/resources/resource_system.h"
#include "opengl_texture.h"

namespace solar {

	opengl_texture_factory::opengl_texture_factory(resource_system& resource_system)
		: _resource_system(resource_system)
		, _is_setup(false) {
	}

	opengl_texture_factory::~opengl_texture_factory() {
		ASSERT(!_is_setup);
		ASSERT(_textures.empty());
	}

	void opengl_texture_factory::setup() {
		ASSERT(!_is_setup);
		_is_setup = true;
	}

	void opengl_texture_factory::teardown() {
		ASSERT(_textures.empty());
		_is_setup = false;
	}

	texture* opengl_texture_factory::create_texture(const resource_address& address, const texture_create_params& params) {
		ASSERT(false);
		UNUSED_PARAMETER(address);
		UNUSED_PARAMETER(params);
		return nullptr;

		//ASSERT(_is_setup);

		//auto iter = _textures.find(id);
		//if (iter != _textures.end()) {
		//	//todo - handle texture pools
		//	//if (iter->second->get_texture_pool().get_name() != texture_pool_name) {
		//	//	ALERT("opengl_texture cannot be used in different texture_pools.\n\ntexture_id: '{}'\nrequested_pool_name: '{}'\ncached_pool_name: '{}'", id, texture_pool_name, iter->second->get_texture_pool().get_name());
		//	//}
		//	return iter->second.get();
		//}

		////todo - handle texture pools
		//UNUSED_PARAMETER(texture_pool_name);
		////opengl_texture_pool* texture_pool = nullptr;
		////auto texture_pool_iter = _texture_pools.find(texture_pool_name);
		////if (texture_pool_iter == _texture_pools.end()) {
		////	ALERT("opengl_texture requested a texture_pool that does not exist.\n\ntexture_id: '{}'\nrequested_pool_name: '{}'", id, texture_pool_name);
		////	texture_pool = _null_texture_pool.get();
		////}
		////else {
		////	texture_pool = texture_pool_iter->second.get();
		////}

		//auto address = _resource_system.resolve_address_to_file("texture", "textures", ".dds", id.c_str(), id_source_description.c_str());
		//opengl_texture* new_texture = new opengl_texture(*this, address);
		//_textures[id] = std::unique_ptr<opengl_texture>(new_texture);

		//return new_texture;

	}

	void opengl_texture_factory::release_texture(texture* texture) {
		ASSERT(false); //todo
		UNUSED_PARAMETER(texture);
	}

}
