#include "rect.h"

#include <algorithm>
#include "solar/utility/assert.h"

namespace solar {

	rect::rect()
		: _left(0)
		, _top(0)
		, _right(0)
		, _bottom(0) {
	}

	rect::rect(const rect& copy)
		: _left(copy._left)
		, _top(copy._top)
		, _right(copy._right)
		, _bottom(copy._bottom) {
	}

	rect::rect(const point& top_left, const size& size)
		: _left(top_left._x)
		, _top(top_left._y)
		, _right(top_left._x + size._width)
		, _bottom(top_left._y + size._height) {

		ASSERT(get_width() >= 0);
		ASSERT(get_height() >= 0);
	}

	rect::rect(const point& top_left, const point& bottom_right)
		: _left(top_left._x)
		, _top(top_left._y)
		, _right(bottom_right._x)
		, _bottom(bottom_right._y) {

		ASSERT(get_width() >= 0);
		ASSERT(get_height() >= 0);
	}

	rect::rect(const size& size)
		: _left(0)
		, _top(0)
		, _right(size._width)
		, _bottom(size._height) {

		ASSERT(get_width() >= 0);
		ASSERT(get_height() >= 0);
	}

	rect::rect(int left, int top, int right, int bottom)
		: _left(left)
		, _top(top)
		, _right(right)
		, _bottom(bottom) {

		ASSERT(get_width() >= 0);
		ASSERT(get_height() >= 0);
	}

	int rect::get_left() const {
		return _left;
	}

	int rect::get_top() const {
		return _top;
	}

	int rect::get_right() const {
		return _right;
	}

	int rect::get_bottom() const {
		return _bottom;
	}

	int rect::get_width() const {
		return (_right - _left);
	}

	int rect::get_height() const {
		return (_bottom - _top);
	}

	point rect::get_top_left() const {
		return point(_left, _top);
	}

	point rect::get_top_right() const {
		return point(_right, _top);
	}

	point rect::get_bottom_right() const {
		return point(_right, _bottom);
	}

	point rect::get_bottom_left() const {
		return point(_left, _bottom);
	}

	point rect::get_center() const {
		return point(get_center_x(), get_center_y());
	}

	int rect::get_center_x() const {
		return _left + (get_width() / 2);
	}

	int rect::get_center_y() const {
		return _top + (get_height() / 2);
	}

	size rect::get_size() const {
		return size(_right - _left, _bottom - _top);
	}

	rect& rect::set_width(int width) {
		ASSERT(width >= 0);
		_right = _left + width;
		return *this;
	}

	rect& rect::set_height(int height) {
		ASSERT(height >= 0);
		_bottom = _top + height;
		return *this;
	}

	rect& rect::move_left(int distance) {
		ASSERT(distance >= 0);
		_left += distance;
		_right += distance;
		return *this;
	}

	rect& rect::move_right(int distance) {
		ASSERT(distance >= 0);
		_left -= distance;
		_right -= distance;
		return *this;
	}

	rect& rect::move_up(int distance) {
		ASSERT(distance >= 0);
		_top -= distance;
		_bottom -= distance;
		return *this;
	}

	rect& rect::move_down(int distance) {
		ASSERT(distance >= 0);
		_top += distance;
		_bottom += distance;
		return *this;
	}

	bool rect::is_point_within(const point& p) const {
		return
			p._x >= _left && p._x < _right &&
			p._y >= _top && p._y < _bottom;
	}

	bool rect::operator == (const rect& rhs) const {
		return
			_left == rhs._left &&
			_top == rhs._top &&
			_right == rhs._right &&
			_bottom == rhs._bottom;
	}

	std::ostream& operator<<(std::ostream& os, const rect& r) {
		return os << "[" << r.get_left() << ", " << r.get_top() << ", " << r.get_right() << ", " << r.get_bottom() << "] (" << r.get_width() << " x " << r.get_height() << ")";
	}

	rect make_rect_constrained_within(const rect& constraint, const rect& to_constrain) {
		rect out = to_constrain;
		out.set_width(std::min(out.get_width(), constraint.get_width()));
		out.set_height(std::min(out.get_height(), constraint.get_height()));
		out.move_right(std::max(0, constraint.get_left() - out.get_left()));
		out.move_left(std::max(0, out.get_right() - constraint.get_right()));
		out.move_down(std::max(0, constraint.get_top() - out.get_top()));
		out.move_up(std::max(0, out.get_bottom() - constraint.get_bottom()));
		return out;
	}
}
