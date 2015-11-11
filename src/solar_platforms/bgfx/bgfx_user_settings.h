#pragma once

#include <bgfx/bgfx.h>
#include "bgfx_window_type.h"
#include "solar/utility/property.h"
#include "solar/math/point.h"
#include "solar/math/size.h"

namespace solar {

	struct bgfx_user_settings {
		MAKE_PROPERTY(bgfx_user_settings, bgfx_window_type, window_type, bgfx_window_type::RESIZABLE_WINDOW);
		MAKE_PROPERTY(bgfx_user_settings, size, window_client_size, size(800, 600));
		MAKE_PROPERTY(bgfx_user_settings, point, window_top_left, point(0, 0));
		MAKE_PROPERTY(bgfx_user_settings, bool, is_window_constrainted_to_desktop, true);

		void trace_dump() const;
		bgfx::RendererType::Enum get_RendererType() const;
	};

}