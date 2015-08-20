#include "rectf.h"
#include "solar/utility/assert.h"

namespace solar {

	rectf::rectf()
		: _left(0)
		, _top(0)
		, _right(0)
		, _bottom(0) {
	}

	rectf::rectf(float left, float top, float right, float bottom)
		: _left(left)
		, _top(top)
		, _right(right)
		, _bottom(bottom) {
	}

	float rectf::get_left() const {
		return _left;
	}

	float rectf::get_top() const {
		return _top;
	}

	float rectf::get_right() const {
		return _right;
	}

	float rectf::get_bottom() const {
		return _bottom;
	}

	float rectf::get_width() const {
		return (_right - _left);
	}

	float rectf::get_height() const {
		return (_bottom - _top);
	}

	float rectf::get_center_x() const {
		return _left + (get_width() / 2.f);
	}

	float rectf::get_center_y() const {
		return _top + (get_height() / 2.f);
	}

	rectf& rectf::move_up(float distance) {
		_top -= distance;
		_bottom -= distance;
		return *this;
	}

	rectf& rectf::move_down(float distance) {
		_top += distance;
		_bottom += distance;
		return *this;
	}

	rectf& rectf::set_height_keep_top(float height) {
		ASSERT(height >= 0.f);
		_bottom = _top + height;
		return *this;
	}

}