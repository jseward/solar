#include "d3d9_texture_factory.h"
#include "solar/utility/assert.h"
#include "solar/utility/alert.h"
#include "solar/math/unit_convert.h"
#include "solar/resources/resource_system.h"
#include "d3d9_context.h"
#include "d3d9_texture.h"
#include "d3d9_texture_pool.h"

namespace solar {

	d3d9_texture_factory::d3d9_texture_factory(d3d9_context& context, resource_system& resource_system)
		: _context(context)
		, _resource_system(resource_system)
		, _is_setup(false) {

		_resource_mapped_memory = std::make_unique<resource_mapped_memory>();
		_resource_mapped_memory->set_fixed_buffer_size_in_mb(1.f);

		_null_texture_pool = std::make_unique<d3d9_texture_pool>("null", mb_to_bytes(1.f), true);
	}

	d3d9_texture_factory::~d3d9_texture_factory() {
		ASSERT(!_is_setup);
		ASSERT(_textures.empty());
	}

	void d3d9_texture_factory::setup() {
		ASSERT(!_is_setup);
		_is_setup = true;
		_context.add_device_event_handler(this);
	}

	void d3d9_texture_factory::teardown() {
		remove_all_textures();
		_context.remove_device_event_handler(this);
		_is_setup = false;
	}

	void d3d9_texture_factory::add_texture_pool(const char* texture_pool_name, unsigned int max_size_in_bytes, bool should_create_mip_maps) {
		ASSERT(_texture_pools.find(texture_pool_name) == _texture_pools.end());
		ASSERT(_null_texture_pool->get_name() != texture_pool_name);
		_texture_pools[texture_pool_name] = std::make_unique<d3d9_texture_pool>(texture_pool_name, max_size_in_bytes, should_create_mip_maps);
	}

	void d3d9_texture_factory::remove_all_textures() {
		for (auto& iter : _textures) {
			iter.second->on_device_lost(_context.get_device());
			iter.second->on_device_released(_context.get_device());
		}
		_textures.clear();
		_caching_context.increment();
	}

	texture* d3d9_texture_factory::get_texture(const char* texture_pool_name, const std::string& id, const std::string& id_source_description) {
		ASSERT(_is_setup);

		auto iter = _textures.find(id);
		if (iter != _textures.end()) {
			if (iter->second->get_texture_pool().get_name() != texture_pool_name) {
				ALERT("d3d9_texture cannot be used in different texture_pools.\n\ntexture_id: '{}'\nrequested_pool_name: '{}'\ncached_pool_name: '{}'", id, texture_pool_name, iter->second->get_texture_pool().get_name());
			}
			return iter->second.get();
		}

		d3d9_texture_pool* texture_pool = nullptr;
		auto texture_pool_iter = _texture_pools.find(texture_pool_name);
		if (texture_pool_iter == _texture_pools.end()) {
			ALERT("d3d9_texture requested a texture_pool that does not exist.\n\ntexture_id: '{}'\nrequested_pool_name: '{}'", id, texture_pool_name);
			texture_pool = _null_texture_pool.get();
		}
		else {
			texture_pool = texture_pool_iter->second.get();
		}
		
		auto address = _resource_system.resolve_address("texture", "textures", ".dds", id.c_str(), id_source_description.c_str());
		d3d9_texture* new_texture = new d3d9_texture(*this, *texture_pool, address);
		_textures[id] = std::unique_ptr<d3d9_texture>(new_texture);

		return new_texture;
	}

	const resource_factory_caching_context& d3d9_texture_factory::get_caching_context() const {
		return _caching_context;
	}

	void d3d9_texture_factory::on_device_created(IDirect3DDevice9* device) {
		for (auto& iter : _textures) {
			iter.second->on_device_created(device);
		}
	}

	void d3d9_texture_factory::on_device_released(IDirect3DDevice9* device) {
		for (auto& iter : _textures) {
			iter.second->on_device_released(device);
		}
	}

	void d3d9_texture_factory::on_device_reset(IDirect3DDevice9* device) {
		for (auto& iter : _textures) {
			iter.second->on_device_reset(device);
		}
	}

	void d3d9_texture_factory::on_device_lost(IDirect3DDevice9* device) {
		for (auto& iter : _textures) {
			iter.second->on_device_lost(device);
		}
	}

	d3d9_context& d3d9_texture_factory::get_d3d9_context() {
		return _context;
	}

	resource_system& d3d9_texture_factory::get_resource_system() {
		return _resource_system;
	}

	resource_mapped_memory& d3d9_texture_factory::get_resource_mapped_memory() {
		return *_resource_mapped_memory.get();
	}

}
