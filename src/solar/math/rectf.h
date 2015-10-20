#pragma once

#include "pointf.h"
#include "sizef.h"

namespace solar {

	class rect;

	class rectf {
	private:
		float _left;
		float _top;
		float _right;
		float _bottom;

	public:
		rectf();
		rectf(float left, float top, float right, float bottom);
		rectf(const pointf& top_left, const sizef& size);
		rectf(const pointf& top_left, const pointf& bottom_right);

		float get_left() const;
		float get_top() const;
		float get_right() const;
		float get_bottom() const;
		float get_width() const;
		float get_height() const;
		float get_center_x() const;
		float get_center_y() const;
		pointf get_top_left() const;
		pointf get_top_right() const;
		pointf get_bottom_right() const;
		pointf get_bottom_left() const;

		rectf& move_up(float distance);
		rectf& move_down(float distance);
		rectf& set_height_keep_top(float height);
	};
}