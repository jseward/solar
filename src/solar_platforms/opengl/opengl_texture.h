#pragma once

#include <memory>
#include "solar/rendering/textures/texture.h"
#include "solar/resources/resource_address.h"
#include "solar/resources/resource_mapped_memory.h"

namespace solar {

	class opengl_texture_factory;

	class opengl_texture : public texture {
	private:
		opengl_texture_factory& _texture_factory;
		const resource_address _resource_address;

	public:
		opengl_texture(opengl_texture_factory& texture_factory, const resource_address& resource_address);
		virtual ~opengl_texture();

		virtual size request_size() override;

	private:
		void create_texture();
		void release_texture();
		uint8_t get_mip_levels_to_skip() const;
	};

}