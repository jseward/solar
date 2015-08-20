#include "d3d9_texture.h"

#include "solar/utility/assert.h"
#include "solar/utility/alert.h"
#include "solar/utility/verify.h"
#include "solar/utility/unused_parameter.h"
#include "solar/time/real_time.h"
#include "solar/resources/resource_system.h"
#include "solar/resources/resource_mapped_memory.h"
#include "d3d9_texture_pool.h"
#include "d3d9_texture_factory.h"
#include "d3d9_context.h"
#include "d3d9_release_com_object.h"
#include "d3d9_format_helpers.h"
#include "d3d9_verify.h"
#include "d3d9_error.h"

namespace solar {

	d3d9_texture::d3d9_texture(d3d9_texture_factory& factory, d3d9_texture_pool& texture_pool, const resource_address& resource_address)
		: _factory(factory)
		, _texture_pool(texture_pool)
		, _resource_address(resource_address)
		, _last_request_time(0)
		, _IDirect3DTexture9(nullptr) {
	}

	d3d9_texture::~d3d9_texture() {
		ASSERT(_IDirect3DTexture9 == nullptr);
	}

	d3d9_texture_pool& d3d9_texture::get_texture_pool() const {
		return _texture_pool;
	}

	const resource_address& d3d9_texture::get_resource_address() const {
		return _resource_address;
	}

	size d3d9_texture::request_size() {
		auto texture = request_IDirect3DTexture9();
		if (texture != nullptr) {
			D3DSURFACE_DESC desc;
			D3D9_VERIFY(texture->GetLevelDesc(0, &desc));
			return size(desc.Width, desc.Height);
		}
		return size();
	}

	unsigned long d3d9_texture::get_last_request_time() const {
		return _last_request_time;
	}

	IDirect3DTexture9* d3d9_texture::request_IDirect3DTexture9() {
		_last_request_time = get_real_time_in_ms();
		if (_IDirect3DTexture9 == nullptr) {
			create_IDirect3DTexture9(_factory.get_d3d9_context().get_device());
		}
		return _IDirect3DTexture9;
	}

	void d3d9_texture::release_IDirect3DTexture9() {
		if (_IDirect3DTexture9 != nullptr) {
			_texture_pool.remove_texture(this, get_texture_size_in_bytes());
			d3d9_release_com_object(_IDirect3DTexture9);
		}
	}

	unsigned int d3d9_texture::get_texture_size_in_bytes() const {
		unsigned int bytes = 0;
		IF_VERIFY(_IDirect3DTexture9 != nullptr) {
			for (unsigned int level = 0; level < _IDirect3DTexture9->GetLevelCount(); ++level) {
				D3DSURFACE_DESC desc;
				_IDirect3DTexture9->GetLevelDesc(level, &desc);
				int bit_count = get_format_bit_count(desc.Format);
				bytes += (desc.Width * desc.Height * bit_count) / 8;
			}
		}
		return bytes;
	}

	void d3d9_texture::create_IDirect3DTexture9(IDirect3DDevice9* device) {
		ASSERT(_IDirect3DTexture9 == nullptr);

		if (!_resource_address.empty()) {

			auto& resource_mapped_memory = _factory.get_resource_mapped_memory();
			resource_mapped_memory.lock();
			if (_factory.get_resource_system().read_to_mapped_memory(resource_mapped_memory, _resource_address)) {

				unsigned int mip_levels = D3DX_DEFAULT;
				if (!_texture_pool.has_mip_maps()) {
					mip_levels = 1;
				}

				DWORD mip_filter = D3DX_DEFAULT;
				if (_texture_pool.has_mip_maps()) {
					mip_filter = D3DX_SKIP_DDS_MIP_LEVELS(get_mip_levels_to_skip(), D3DX_FILTER_BOX);
				}

				HRESULT hr = ::D3DXCreateTextureFromFileInMemoryEx(
					device,
					resource_mapped_memory.get_buffer(),
					resource_mapped_memory.get_buffer_size(),
					D3DX_DEFAULT_NONPOW2,
					D3DX_DEFAULT_NONPOW2,
					mip_levels,
					0,
					D3DFMT_UNKNOWN,
					D3DPOOL_MANAGED,
					D3DX_DEFAULT,
					mip_filter,
					0,
					NULL,
					NULL,
					&_IDirect3DTexture9);
				if (FAILED(hr)) {
					ALERT("Failed to create texture.\n\naddress: {}\nerror: {}", _resource_address.to_string(), make_d3d9_error_message(hr));
				}
				else {
					_texture_pool.add_texture(this, get_texture_size_in_bytes());
				}
			}
			resource_mapped_memory.unlock();

		}
	}

	int d3d9_texture::get_mip_levels_to_skip() const {
		//todo - texture registry
		return 0;
	}

	void d3d9_texture::on_device_created(IDirect3DDevice9* device) {
		UNUSED_PARAMETER(device);
		//do nothing, textures are always created on demand when requested so they can be pooled.
		ASSERT(_IDirect3DTexture9 == nullptr);
	}

	void d3d9_texture::on_device_released(IDirect3DDevice9* device) {
		UNUSED_PARAMETER(device);
		release_IDirect3DTexture9();
	}

	void d3d9_texture::on_device_reset(IDirect3DDevice9* device) {
		UNUSED_PARAMETER(device);
	}

	void d3d9_texture::on_device_lost(IDirect3DDevice9* device) {
		UNUSED_PARAMETER(device);
	}

}