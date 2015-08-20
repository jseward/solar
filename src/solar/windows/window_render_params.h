#pragma once

#include "solar/math/point.h"

namespace solar {

	class render_device;
	class window_renderer;

	class window_render_params {
	private:
		render_device& _render_device;
		window_renderer& _window_renderer;
		point _cursor_pos;

	public:
		window_render_params(render_device& render_device, window_renderer& window_renderer, const point& cursor_pos)
			: _render_device(render_device)
			, _window_renderer(window_renderer)
			, _cursor_pos(cursor_pos) {
		}

		render_device& get_render_device() const {
			return _render_device;
		}

		window_renderer& get_window_renderer() const {
			return _window_renderer;
		}

		const point& get_cursor_pos() const {
			return _cursor_pos;
		}
	};

}