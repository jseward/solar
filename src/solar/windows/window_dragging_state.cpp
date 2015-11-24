#include "window_dragging_state.h"

#include "solar/utility/assert.h"

namespace solar {

	window_dragging_state::window_dragging_state()
		: _is_dragging(false) {
	}

	void window_dragging_state::begin_dragging(const point& cursor_pos) {
		
		//too easy to mess up state by dragging outside of window client area, just ignore duplicate begins.
		//ASSERT(!_is_dragging); 

		_is_dragging = true;
		_begin_cursor_pos = cursor_pos;
		_last_cursor_pos = cursor_pos;
	}

	void window_dragging_state::end_dragging() {
		_is_dragging = false;
		_begin_cursor_pos = point();
		_last_cursor_pos = point();
	}
}