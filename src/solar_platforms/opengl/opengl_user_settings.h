#pragma once

#include "opengl_window_type.h"
#include "solar/utility/property.h"
#include "solar/math/point.h"
#include "solar/math/size.h"

namespace solar {

	struct opengl_user_settings {
		MAKE_PROPERTY(opengl_user_settings, opengl_window_type, window_type, opengl_window_type::RESIZABLE_WINDOW);
		MAKE_PROPERTY(opengl_user_settings, size, window_client_size, size(800, 600));
		MAKE_PROPERTY(opengl_user_settings, point, window_top_left, point(0, 0));
		MAKE_PROPERTY(opengl_user_settings, bool, is_window_constrainted_to_desktop, true);

		void trace_dump() const;
	};

}