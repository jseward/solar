#pragma once

#include "mouse_button.h"

namespace solar {

	class mouse_device_event_handler {
	public:
		virtual void on_mouse_button_down(mouse_button button) = 0;
		virtual void on_mouse_button_up(mouse_button button) = 0;
		virtual void on_mouse_button_double_clicked(mouse_button button) = 0;
		virtual void on_mouse_moved(int new_x, int new_y) = 0;
		virtual void on_mouse_moved_high_res(int delta_x, int delta_y) = 0;
		virtual void on_mouse_wheel_moved(float delta) = 0;
	};

}