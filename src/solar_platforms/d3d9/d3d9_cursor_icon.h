#pragma once

#include "d3d9_headers.h"
#include "d3d9_cursor_icon_def.h"
#include "solar/resources/resource_address.h"
#include "solar/rendering/cursors/cursor_icon.h"
#include "solar/math/point.h"

namespace solar {

	class d3d9_cursor_icon_factory;

	class d3d9_cursor_icon : public cursor_icon {
	private:
		d3d9_cursor_icon_factory& _factory;
		resource_address _def_address;
		d3d9_cursor_icon_def _def;
		IDirect3DSurface9* _device_surface;

	public:
		d3d9_cursor_icon(d3d9_cursor_icon_factory& factory, const resource_address& address);
		virtual ~d3d9_cursor_icon();

		void load_def();

		const point& get_hotspot_offset() const;
		IDirect3DSurface9* get_device_surface() const;

		void on_device_created(IDirect3DDevice9* device);
		void on_device_released(IDirect3DDevice9* device);
		void on_device_reset(IDirect3DDevice9* device);
		void on_device_lost(IDirect3DDevice9* device);

	private:
		void create_device_surface(IDirect3DDevice9* device);
		void release_device_surface();
	};

}