#include "window_render_state_helpers.h"
#include "solar/windows/window.h"
#include "solar/windows/rendering/window_render_params.h"

namespace solar {

	window_render_state make_best_window_render_state(const window& window, const window_render_params& params) {
		return make_best_window_render_state(window, params, false);
	}

	window_render_state make_best_window_render_state(const window& window, const window_render_params& params, bool is_pressed) {
		window_render_state state = window_render_state::NORMAL;
		
		if (!window.is_enabled()) {
			state = window_render_state::DISABLED;
		}
		else if (is_pressed) {
			state = window_render_state::PRESSED;
		}
		else if (window.get_area().is_point_within(params._cursor_pos)) {
			state = window_render_state::HOVERED;
		}
		else if (window.is_focused()) {
			state = window_render_state::FOCUSED;
		}

		return state;
	}

}