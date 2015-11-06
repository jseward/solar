#pragma once

#include "solar/input/keyboard_device.h"

namespace solar {

	class window_mouse_wheel_event_params {
	public:
		float _delta;
		point _cursor_pos;
		const keyboard_device& _keyboard_device;

	public:
		window_mouse_wheel_event_params(float delta, const point& cursor_pos, const keyboard_device& keyboard_device)
			: _delta(delta)
			, _cursor_pos(cursor_pos)
			, _keyboard_device(keyboard_device) {
		}

		bool is_shift_down() const {
			return _keyboard_device.is_shift_down();
		}
	};

}
