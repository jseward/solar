#pragma once

#include <string>

namespace solar {

	class window;
	class color;
	class size;

	//ABOUT: helpful functions to reduce the bloat of window.h, not needed to be member functions of window
	//but very useful.

	extern std::string make_window_hierarchy_id_string(const window& window);
	extern color make_window_alpha_scaled_color(const window& window, const color& color);

	//calculate_best_area_scale is very useful for top level windows that need to automatically scale their children
	//when resized (ex. hud, front end). use this function in on_parent_area_changed() to set the area_scale of the 
	//window appropriately.
	extern float calculate_best_area_scale(const size& desired_size, const size& current_size);

}