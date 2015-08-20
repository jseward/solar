#include "window_layout_anchors.h"
#include <memory>
#include "solar/archiving/archiving_helpers.h"

namespace solar {

	window_layout_anchors::window_layout_anchors() 
		: _left(false)
		, _right(false)
		, _top(false)
		, _bottom(false) {
	}

	bool window_layout_anchors::has_left() const {
		return _left;
	}

	bool window_layout_anchors::has_right() const {
		return _right;
	}

	bool window_layout_anchors::has_top() const {
		return _top;
	}

	bool window_layout_anchors::has_bottom() const {
		return _bottom;
	}

	void window_layout_anchors::read_from_archive(archive_reader& reader, const char* name) {
		int left_v = 0;
		int right_v = 0;
		int top_v = 0;
		int bottom_v = 0;
		read_ints(reader, name, left_v, top_v, right_v, bottom_v);
		_left = left_v != 0;
		_top = top_v != 0;
		_right = right_v != 0;
		_bottom = bottom_v != 0;
	}

	void window_layout_anchors::write_to_archive(archive_writer& writer, const char* name) const {
		int left_v = _left ? 1 : 0;
		int right_v = _right ? 1 : 0;
		int top_v = _top ? 1 : 0;
		int bottom_v = _bottom ? 1 : 0;
		write_ints(writer, name, left_v, top_v, right_v, bottom_v);
	}

}
