#pragma once

#include "solar/input/mouse_button.h"
#include "solar/input/keyboard_device.h"
#include "solar/math/point.h"

namespace solar {

	class window_mouse_button_event_params {
	public:
		mouse_button _mouse_button;
		point _cursor_pos;
		const keyboard_device& _keyboard_device;

	public:
		window_mouse_button_event_params(mouse_button mouse_button, const point& cursor_pos, const keyboard_device& keyboard_device)
			: _mouse_button(mouse_button)
			, _cursor_pos(cursor_pos) 
			, _keyboard_device(keyboard_device) {
		}

		bool is_shift_down() const {
			return _keyboard_device.is_shift_down();
		}
	};

}
