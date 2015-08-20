#include "window_layout_margins.h"
#include "solar/archiving/archiving_helpers.h"

namespace solar {

	window_layout_margins::window_layout_margins()
		: _left(0)
		, _right(0)
		, _top(0)
		, _bottom(0) {
	}

	window_layout_margins::window_layout_margins(int left, int right, int top, int bottom)
		: _left(left)
		, _right(right)
		, _top(top)
		, _bottom(bottom) {
	}

	int window_layout_margins::get_left() const {
		return _left;
	}

	int window_layout_margins::get_right() const {
		return _right;
	}

	int window_layout_margins::get_top() const {
		return _top;
	}

	int window_layout_margins::get_bottom() const {
		return _bottom;
	}

	window_layout_margins window_layout_margins::operator*(float rhs) const {
		return window_layout_margins(
			static_cast<int>(_left * rhs),
			static_cast<int>(_right * rhs),
			static_cast<int>(_top * rhs),
			static_cast<int>(_bottom * rhs));
	}

	void window_layout_margins::read_from_archive(archive_reader& reader, const char* name) {
		read_ints(reader, name, _left, _top, _right, _bottom);
	}

	void window_layout_margins::write_to_archive(archive_writer& writer, const char* name) const {
		write_ints(writer, name, _left, _top, _right, _bottom);
	}

}
