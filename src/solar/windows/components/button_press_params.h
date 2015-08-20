#pragma once

#include "solar/input/mouse_button.h"

namespace solar {

	class button;

	class button_press_params {
	public:
		button& _button;
		mouse_button _mouse_button;

	public:
		button_press_params(button& button) 
			: _button(button) 
			, _mouse_button(mouse_button::invalid) {
		}

		button_press_params& set_mouse_button(mouse_button mouse_button) {
			_mouse_button = mouse_button;
			return *this;
		}
	};

}