#pragma once

#include "d3d9_headers.h"
#include "solar/rendering/textures/texture.h"
#include "solar/resources/resource_address.h"

namespace solar {

	class d3d9_texture_factory;
	class d3d9_texture_pool;

	class d3d9_texture : public texture {
	private:
		d3d9_texture_factory& _factory;
		d3d9_texture_pool& _texture_pool;
		resource_address _resource_address;
		mutable unsigned long _last_request_time;
		IDirect3DTexture9* _IDirect3DTexture9;

	public:
		d3d9_texture(d3d9_texture_factory& factory, d3d9_texture_pool& texture_pool, const resource_address& resource_address);
		virtual ~d3d9_texture();

		virtual size request_size() override;

		d3d9_texture_pool& get_texture_pool() const;
		const resource_address& get_resource_address() const;
		unsigned long get_last_request_time() const;
		IDirect3DTexture9* request_IDirect3DTexture9();
		void release_IDirect3DTexture9();
		unsigned int get_texture_size_in_bytes() const;

		void on_device_created(IDirect3DDevice9* device);
		void on_device_released(IDirect3DDevice9* device);
		void on_device_reset(IDirect3DDevice9* device);
		void on_device_lost(IDirect3DDevice9* device);

	private:
		void create_IDirect3DTexture9(IDirect3DDevice9* device);
		int get_mip_levels_to_skip() const;
	};

}