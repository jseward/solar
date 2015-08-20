#pragma once

#include "d3d9_headers.h"

namespace solar {

	class d3d9_device_event_handler {
	public:
		virtual void on_device_created(IDirect3DDevice9* device) = 0;
		virtual void on_device_released(IDirect3DDevice9* device) = 0;
		virtual void on_device_reset(IDirect3DDevice9* device) = 0;
		virtual void on_device_lost(IDirect3DDevice9* device) = 0;
	};

}