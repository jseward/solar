#include "d3d9_texture_pool.h"
#include "solar/containers/container_helpers.h"
#include "solar/utility/trace.h"
#include "d3d9_texture.h"

namespace solar {

	d3d9_texture_pool::d3d9_texture_pool(const std::string& name, unsigned int max_size_in_bytes, bool has_mip_maps)
		: _name(name) 
		, _max_size_in_bytes(max_size_in_bytes)
		, _has_mip_maps(has_mip_maps)
		, _used_size_in_bytes(0) {
	}

	d3d9_texture_pool::~d3d9_texture_pool() {
		ASSERT(_textures.empty());
		ASSERT(_used_size_in_bytes == 0);
	}

	bool d3d9_texture_pool::has_mip_maps() const {
		return _has_mip_maps;
	}

	const std::string& d3d9_texture_pool::get_name() const {
		return _name;
	}

	void d3d9_texture_pool::add_texture(d3d9_texture* texture, unsigned int texture_size_in_bytes) {
		push_back_and_verify_not_found(_textures, texture);
		_used_size_in_bytes += texture_size_in_bytes;
		if (_used_size_in_bytes > _max_size_in_bytes) {
			TRACE("WARNING: d3d9_texture_pool max_size exceeded {{ name:{} , max_size_in_bytes:{} , used_size_in_bytes:{} }}", _name, _max_size_in_bytes, _used_size_in_bytes);
			remove_oldest_textures();
		}
	}

	void d3d9_texture_pool::remove_oldest_textures() {
		std::sort(_textures.begin(), _textures.end(), [](d3d9_texture* a, d3d9_texture* b) {
			return a->get_last_request_time() < b->get_last_request_time();
		});

		while (_used_size_in_bytes > _max_size_in_bytes && _textures.size() > 1) {
			TRACE("d3d9_texture_pool removing oldest texture : {}", _textures.at(0)->get_resource_address().to_string());
			_textures.at(0)->release_IDirect3DTexture9();
		}
	}

	void d3d9_texture_pool::remove_texture(d3d9_texture* texture, unsigned int texture_size_in_bytes) {
		find_and_erase(_textures, texture);
		ASSERT(texture_size_in_bytes <= _used_size_in_bytes);
		_used_size_in_bytes -= texture_size_in_bytes;
	}

}