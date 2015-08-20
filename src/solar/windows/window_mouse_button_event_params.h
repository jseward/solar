#pragma once

#include "solar/input/mouse_button.h"
#include "solar/math/point.h"

namespace solar {

	class window_mouse_button_event_params {
	public:
		mouse_button _mouse_button;
		point _cursor_pos;

	public:
		window_mouse_button_event_params(mouse_button mouse_button, const point& cursor_pos)
			: _mouse_button(mouse_button)
			, _cursor_pos(cursor_pos) {
		}
	};

}