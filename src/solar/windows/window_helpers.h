#pragma once

#include <string>

namespace solar {

	class window;
	class color;

	//ABOUT: helpful functions to reduce the bloat of window.h, not needed to be member functions of window
	//but very useful.

	extern std::string make_window_hierarchy_id_string(const window& window);
	extern color make_window_alpha_scaled_color(const window& window, const color& color);

}