#pragma once

#include <ostream>
#include "point.h"
#include "size.h"

namespace solar {

	class rect {
	private:
		int _left;
		int _top;
		int _right;
		int _bottom;

	public:
		rect();
		rect(const rect& copy);
		rect(const point& top_left, const size& size);
		rect(const point& top_left, const point& bottom_right);
		rect(const size& size);
		rect(int left, int top, int right, int bottom);

		int get_left() const;
		int get_top() const;
		int get_right() const;
		int get_bottom() const;
		int get_width() const;
		int get_height() const;
		int get_center_x() const;
		int get_center_y() const;
		point get_top_left() const;
		point get_top_right() const;
		point get_bottom_right() const;
		point get_bottom_left() const;
		point get_center() const;
		size get_size() const;

		rect& set_width(int width);
		rect& set_height(int height);
		rect& move_left(int distance);
		rect& move_right(int distance);
		rect& move_up(int distance);
		rect& move_down(int distance);

		bool is_point_within(const point& p) const;

		bool operator==(const rect& rhs) const;

	public:
		friend std::ostream& operator<<(std::ostream& os, const rect& r);
		friend rect make_rect_constrained_within(const rect& constraint, const rect& to_constrain);
	};
}