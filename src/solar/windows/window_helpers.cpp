#include "window_helpers.h"

#include "window.h"
#include "solar/rendering/color.h"
#include "solar/math/size.h"
#include "solar/utility/type_convert.h"

namespace solar {

	std::string make_window_hierarchy_id_string(const window& w) {
		if (!w.has_parent()) {
			return w.get_id();
		}
		else {
			std::string s = w.get_id();
			const window* cw = &w;
			while (cw->has_parent()) {
				cw = &cw->get_parent();
				s.insert(0, ".");
				s.insert(0, cw->get_id());
			}
			return s;
		}
	}

	color make_window_alpha_scaled_color(const window& window, const color& c) {
		return color(c, c.get_a() * window.get_alpha());
	}

	float calculate_best_area_scale(const size& desired_size, const size& current_size) {
		if (
			desired_size._width <= 0 || desired_size._height <= 0 ||
			current_size._width <= 0 || current_size._height <= 0) {
			return 1.f;
		}

		float desired_aspect_ratio = int_to_float(desired_size._width) / int_to_float(desired_size._height);
		float current_aspect_ratio = int_to_float(current_size._width) / int_to_float(current_size._height);

		if (current_aspect_ratio >= desired_aspect_ratio) {
			return int_to_float(current_size._height) / int_to_float(desired_size._height);
		}
		else {
			return int_to_float(current_size._width) / int_to_float(desired_size._width);
		}
	}

}