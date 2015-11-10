#include "bgfx_texture.h"

#include "solar/utility/assert.h"
#include "solar/utility/unused_parameter.h"
#include "solar/resources/resource_system.h"
#include "bgfx_texture_factory.h"

namespace solar {

	bgfx_texture::bgfx_texture(bgfx_texture_factory& texture_factory, const resource_address& resource_address)
		: _texture_factory(texture_factory)
		, _resource_address(resource_address)
		, _handle(BGFX_INVALID_HANDLE) {
	}

	bgfx_texture::~bgfx_texture() {
		release_texture();
	}

	size bgfx_texture::request_size() {
		if (!isValid(_handle)) {
			create_texture();
		}
		return size(_info.width, _info.height);
	}

	void bgfx_texture::create_texture() {
		if (!_resource_address.empty()) {
			ASSERT(!isValid(_handle));
			ASSERT(_resource_address.get_file_extension() == ".dds"); //bgfx only supports DDS, KTX or PVR texture data

			_resource_mapped_memory.lock();
			if (_texture_factory.get_resource_system().read_to_mapped_memory(_resource_mapped_memory, _resource_address)) {
				auto mem = bgfx::makeRef(
					_resource_mapped_memory.get_buffer(),
					_resource_mapped_memory.get_buffer_size(),
					&bgfx_texture::unlock_resource_mapped_memory,
					this);
				_handle = bgfx::createTexture(mem, BGFX_TEXTURE_NONE, get_mip_levels_to_skip(), &_info);
			}
		}
	}

	void bgfx_texture::release_texture() {
		if (isValid(_handle)) {
			bgfx::destroyTexture(_handle);
			_handle = BGFX_INVALID_HANDLE;
		}
	}

	uint8_t bgfx_texture::get_mip_levels_to_skip() const {
		//todo - texture registry
		return 0;
	}

	void bgfx_texture::unlock_resource_mapped_memory(void* ptr, void* user_data) {
		UNUSED_PARAMETER(ptr);
		auto texture = reinterpret_cast<bgfx_texture*>(user_data);
		texture->_resource_mapped_memory.unlock();
	}

}