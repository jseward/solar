#include "rectf.h"

#include "solar/utility/assert.h"
#include "solar/utility/type_convert.h"
#include "rect.h"

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

	rectf::rectf(const pointf& top_left, const sizef& size) 
		: _left(top_left._x)
		, _top(top_left._y)
		, _right(top_left._x + size._width)
		, _bottom(top_left._y + size._height) {
	}

	rectf::rectf(const pointf& top_left, const pointf& bottom_right)
		: _left(top_left._x)
		, _top(top_left._y)
		, _right(bottom_right._x)
		, _bottom(bottom_right._y) {
	}

	void rectf::set_left(float left) {
		_left = left;
	}

	void rectf::set_right(float right) {
		_right = right;
	}

	void rectf::set_top(float top) {
		_top = top;
	}

	void rectf::set_bottom(float bottom) {
		_bottom = bottom;
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

	pointf rectf::get_top_left() const {
		return pointf(_left, _top);
	}

	pointf rectf::get_top_right() const {
		return pointf(_right, _top);
	}

	pointf rectf::get_bottom_right() const {
		return pointf(_right, _bottom);
	}

	pointf rectf::get_bottom_left() const {
		return pointf(_left, _bottom);
	}

	pointf rectf::get_center() const {
		return pointf(get_center_x(), get_center_y());
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