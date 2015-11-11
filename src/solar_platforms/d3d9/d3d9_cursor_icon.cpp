#include "d3d9_cursor_icon.h"

#include "solar/utility/assert.h"
#include "solar/utility/unused_parameter.h"
#include "solar/resources/resource_mapped_memory.h"
#include "solar/resources/resource_system.h"
#include "d3d9_verify.h"
#include "d3d9_cursor.h"
#include "d3d9_cursor_icon_factory.h"
#include "d3d9_release_com_object.h"

namespace solar {

	d3d9_cursor_icon::d3d9_cursor_icon(d3d9_cursor_icon_factory& factory, const resource_address& def_address)
		: _factory(factory)
		, _def_address(def_address)
		, _def()
		, _device_surface(nullptr) {
	}

	d3d9_cursor_icon::~d3d9_cursor_icon() {
		ASSERT(_device_surface == nullptr);
	}

	void d3d9_cursor_icon::load_def() {
		_factory.get_resource_system().read_object_as_json(_def, _def_address);
	}

	const point& d3d9_cursor_icon::get_hotspot_offset() const {
		return _def._hotspot_offset;
	}

	IDirect3DSurface9* d3d9_cursor_icon::get_device_surface() const {
		return _device_surface;
	}

	void d3d9_cursor_icon::on_device_created(IDirect3DDevice9* device) {
		UNUSED_PARAMETER(device);
	}

	void d3d9_cursor_icon::on_device_released(IDirect3DDevice9* device) {
		UNUSED_PARAMETER(device);
	}

	void d3d9_cursor_icon::on_device_reset(IDirect3DDevice9* device) {
		create_device_surface(device);
	}

	void d3d9_cursor_icon::on_device_lost(IDirect3DDevice9* device) {
		UNUSED_PARAMETER(device);
		release_device_surface();
	}

	void d3d9_cursor_icon::create_device_surface(IDirect3DDevice9* device) {
		auto texture_address = _factory.get_resource_system().resolve_address("d3d9_cursor_icon_texture", "textures", ".dds", _def._texture_name.c_str(), _def_address.to_string().c_str());

		if (!_def._texture_name.empty()) {
			auto& resource_mapped_memory = _factory.get_resource_mapped_memory();
			resource_mapped_memory.lock();
			if (_factory.get_resource_system().read_to_mapped_memory(resource_mapped_memory, texture_address)) {

				ASSERT(_device_surface == nullptr);
				D3D9_VERIFY(device->CreateOffscreenPlainSurface(
					32U, 32U, //NOTE: Direct3D requires a cursor be 32x32 pixels.
					D3DFMT_A8R8G8B8,
					D3DPOOL_DEFAULT,
					&_device_surface,
					NULL));

				D3DXIMAGE_INFO ii;
				D3D9_VERIFY(::D3DXLoadSurfaceFromFileInMemory(
					_device_surface,
					NULL,
					NULL,
					resource_mapped_memory.get_buffer(),
					resource_mapped_memory.get_buffer_size(),
					NULL,
					D3DX_DEFAULT,
					0xFF000000L,
					&ii));
			}
			resource_mapped_memory.unlock();
		}

		_factory.get_cursor().on_icon_device_surface_changed(*this);
	}

	void d3d9_cursor_icon::release_device_surface() {
		if (_device_surface != nullptr) {
			d3d9_release_com_object(_device_surface);
		}

		_factory.get_cursor().on_icon_device_surface_changed(*this);
	}

}