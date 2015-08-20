#pragma once

#include "solar/windows/window_render_state.h"

namespace solar {

	class window;
	class window_render_params;

	extern window_render_state get_best_window_render_state(const window& window, const window_render_params& params);
	extern window_render_state get_best_window_render_state(const window& window, const window_render_params& params, bool is_pressed);

}