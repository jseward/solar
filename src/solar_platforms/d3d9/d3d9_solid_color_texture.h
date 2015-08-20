#pragma once

#include "d3d9_headers.h"
#include "solar/rendering/color.h"

namespace solar {

	class d3d9_solid_color_texture {
	private:
		IDirect3DTexture9* _IDirect3DTexture9;
		color _color;

	public:
		d3d9_solid_color_texture(const color& color);
		~d3d9_solid_color_texture();

		IDirect3DTexture9* get();

		void on_device_created(IDirect3DDevice9* device);
		void on_device_released(IDirect3DDevice9* device);
		void on_device_reset(IDirect3DDevice9* device);
		void on_device_lost(IDirect3DDevice9* device);
	};

}