#include "d3d9_solid_color_texture.h"

#include "solar/utility/assert.h"
#include "solar/utility/unused_parameter.h"
#include "d3d9_release_com_object.h"
#include "d3d9_verify.h"
#include "d3d9_surface_helpers.h"

namespace solar {

	d3d9_solid_color_texture::d3d9_solid_color_texture(const color& color) 
		: _IDirect3DTexture9(nullptr)
		, _color(color) {
	}

	d3d9_solid_color_texture::~d3d9_solid_color_texture() {
		ASSERT(_IDirect3DTexture9 == nullptr);
	}

	IDirect3DTexture9* d3d9_solid_color_texture::get() {
		return _IDirect3DTexture9;
	}

	void d3d9_solid_color_texture::on_device_created(IDirect3DDevice9* device) {
		UNUSED_PARAMETER(device);
	}

	void d3d9_solid_color_texture::on_device_released(IDirect3DDevice9* device) {
		UNUSED_PARAMETER(device);
	}

	void d3d9_solid_color_texture::on_device_reset(IDirect3DDevice9* device) {
		ASSERT(_IDirect3DTexture9 == NULL);
		D3D9_VERIFY(device->CreateTexture(1, 1, 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &_IDirect3DTexture9, NULL));

		IDirect3DTexture9* scratch_pad_texture = nullptr;
		D3D9_VERIFY(device->CreateTexture(1, 1, 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &scratch_pad_texture, NULL));
		fill_texture_surface(scratch_pad_texture, 0, _color.to_argb32());
		D3D9_VERIFY(device->UpdateTexture(scratch_pad_texture, _IDirect3DTexture9));
		d3d9_release_com_object(scratch_pad_texture);
	}

	void d3d9_solid_color_texture::on_device_lost(IDirect3DDevice9* device) {
		UNUSED_PARAMETER(device);
		d3d9_release_com_object(_IDirect3DTexture9);
	}

}
