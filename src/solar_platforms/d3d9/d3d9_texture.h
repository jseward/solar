#pragma once

#include "d3d9_headers.h"
#include "solar/rendering/textures/texture.h"
#include "solar/resources/resource_address.h"
#include "solar/utility/ref_countable.h"

namespace solar {

	class d3d9_texture_factory;

	class d3d9_texture 
		: public texture
		, public ref_countable {

	private:
		d3d9_texture_factory& _factory;
		const resource_address _address;
		const texture_create_params _create_params;

		mutable unsigned long _last_request_time;
		IDirect3DTexture9* _IDirect3DTexture9;

	public:
		d3d9_texture(d3d9_texture_factory& factory, const resource_address& address, const texture_create_params& create_params);
		virtual ~d3d9_texture();

		virtual size request_size() override;

		const resource_address& get_address() const;
		const texture_create_params& get_create_params() const;
		IDirect3DTexture9* request_IDirect3DTexture9();
		void release_IDirect3DTexture9();
		unsigned long get_last_request_time() const;
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
