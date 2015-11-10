#pragma once

#include <memory>
#include <bgfx/bgfx.h>
#include "solar/rendering/textures/texture.h"
#include "solar/resources/resource_address.h"
#include "solar/resources/resource_mapped_memory.h"

namespace solar {

	class bgfx_texture_factory;

	class bgfx_texture : public texture {
	private:
		bgfx_texture_factory& _texture_factory;
		const resource_address _resource_address;

		resource_mapped_memory _resource_mapped_memory;
		bgfx::TextureHandle _handle;
		bgfx::TextureInfo _info;

	public:
		bgfx_texture(bgfx_texture_factory& texture_factory, const resource_address& resource_address);
		virtual ~bgfx_texture();

		virtual size request_size() override;

	private:
		void create_texture();
		void release_texture();
		uint8_t get_mip_levels_to_skip() const;

	private:
		static void unlock_resource_mapped_memory(void* ptr, void* user_data);
	};

}