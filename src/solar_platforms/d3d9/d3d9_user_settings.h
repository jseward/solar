#pragma once

#include "d3d9_headers.h"
#include "d3d9_window_type.h"
#include "solar/utility/property.h"
#include "solar/math/point.h"
#include "solar/math/size.h"

namespace solar {

	struct d3d9_user_settings {
		MAKE_PROPERTY(d3d9_user_settings, d3d9_window_type, window_type, d3d9_window_type::RESIZABLE_WINDOW);
		MAKE_PROPERTY(d3d9_user_settings, point, window_top_left, point(0, 0));
		MAKE_PROPERTY(d3d9_user_settings, bool, is_window_constrainted_to_desktop, true);
		MAKE_PROPERTY(d3d9_user_settings, bool, is_cursor_clipped, false);
		MAKE_PROPERTY(d3d9_user_settings, unsigned int, adapter_ordinal, D3DADAPTER_DEFAULT);
		MAKE_PROPERTY(d3d9_user_settings, D3DFORMAT, adapter_format, D3DFMT_X8R8G8B8);
		MAKE_PROPERTY(d3d9_user_settings, D3DFORMAT, depth_stencil_format, D3DFMT_D24S8);
		MAKE_PROPERTY(d3d9_user_settings, D3DFORMAT, backbuffer_format, D3DFMT_A8R8G8B8);
		MAKE_PROPERTY(d3d9_user_settings, size, backbuffer_size, size(640, 480));
		MAKE_PROPERTY(d3d9_user_settings, unsigned int, fullscreen_refresh_rate, 60);

		void trace_dump() const;
	};

}