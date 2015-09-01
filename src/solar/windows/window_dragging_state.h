#pragma once

#include "solar/math/point.h"

namespace solar {

	class window_dragging_state {
	public:
		bool _is_dragging;
		point _begin_cursor_pos;

	public:
		window_dragging_state();
		void begin_dragging(const point& cursor_pos);
		void end_dragging();
	};

}