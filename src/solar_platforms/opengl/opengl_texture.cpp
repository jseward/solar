#include "opengl_texture.h"

#include "solar/utility/assert.h"
#include "solar/utility/unused_parameter.h"
#include "solar/resources/resource_system.h"
#include "opengl_texture_factory.h"

namespace solar {

	opengl_texture::opengl_texture(opengl_texture_factory& texture_factory, const resource_address& resource_address)
		: _texture_factory(texture_factory)
		, _resource_address(resource_address) {
	}

	opengl_texture::~opengl_texture() {
		release_texture();
	}

	size opengl_texture::request_size() {
		ASSERT(false);
		return size(0, 0);
	}

	void opengl_texture::create_texture() {
		ASSERT(false);
		//if (!_resource_address.empty()) {
		//	ASSERT(!isValid(_handle));
		//	ASSERT(_resource_address.get_file_extension() == ".dds"); //opengl only supports DDS, KTX or PVR texture data

		//	_resource_mapped_memory.lock();
		//	if (_texture_factory.get_resource_system().read_to_mapped_memory(_resource_mapped_memory, _resource_address)) {
		//		auto mem = opengl::makeRef(
		//			_resource_mapped_memory.get_buffer(),
		//			_resource_mapped_memory.get_buffer_size(),
		//			&opengl_texture::unlock_resource_mapped_memory,
		//			this);
		//		_handle = opengl::createTexture(mem, opengl_TEXTURE_NONE, get_mip_levels_to_skip(), &_info);
		//	}
		//}
	}

	void opengl_texture::release_texture() {
		ASSERT(false);
		//if (isValid(_handle)) {
		//	opengl::destroyTexture(_handle);
		//	_handle = opengl_INVALID_HANDLE;
		//}
	}

	uint8_t opengl_texture::get_mip_levels_to_skip() const {
		//todo - texture registry
		return 0;
	}

}