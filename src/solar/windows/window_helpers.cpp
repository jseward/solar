#include "window_helpers.h"
#include "window.h"
#include "solar/rendering/color.h"

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
}