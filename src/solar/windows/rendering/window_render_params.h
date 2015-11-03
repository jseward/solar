#pragma once

#include "solar/math/point.h"

namespace solar {

	class render_device;
	class window_renderer;

	class window_render_params {
	public:
		render_device& _render_device;
		window_renderer& _window_renderer;
		point _cursor_pos;
		float _time_elapsed;

	public:
		window_render_params(render_device& render_device, window_renderer& window_renderer, const point& cursor_pos, float time_elapsed)
			: _render_device(render_device)
			, _window_renderer(window_renderer)
			, _cursor_pos(cursor_pos) 
			, _time_elapsed(time_elapsed) {
		}
	};

}