#include "window_layout.h"
#include "solar/windows/window.h"
#include "solar/utility/assert.h"
#include "solar/archiving/archiving_helpers.h"
#include "solar/archiving/archiving_math_helpers.h"
#include <algorithm>

namespace solar {

	rect window_layout::build_area(const rect& parent_area, float scale) const {
		ASSERT(scale > 0.f);

		auto scaled_margins = _margins * scale;
		auto scaled_size = _size * scale;

		int left = 0;
		int right = 0;
		if (_anchors.has_left() && _anchors.has_right()) {
			left = parent_area.get_left() + scaled_margins.get_left();
			right = std::max(left, parent_area.get_right() - scaled_margins.get_right());
		}
		else if (_anchors.has_left()) {
			left = parent_area.get_left() + scaled_margins.get_left();
			right = left + scaled_size.get_width();
		}
		else if (_anchors.has_right()) {
			right = parent_area.get_right() - scaled_margins.get_right();
			left = right - scaled_size.get_width();
		}
		else {
			left = parent_area.get_center().get_x() - (scaled_size.get_width() / 2);
			right = left + scaled_size.get_width();
		}

		int top = 0;
		int bottom = 0;
		if (_anchors.has_top() && _anchors.has_bottom()) {
			top = parent_area.get_top() + scaled_margins.get_top();
			bottom = std::max(top, parent_area.get_bottom() - scaled_margins.get_bottom());
		}
		else if (_anchors.has_top()) {
			top = parent_area.get_top() + scaled_margins.get_top();
			bottom = top + scaled_size.get_height();
		}
		else if (_anchors.has_bottom()) {
			bottom = parent_area.get_bottom() - scaled_margins.get_bottom();
			top = bottom - scaled_size.get_height();
		}
		else {
			top = parent_area.get_center().get_y() - (scaled_size.get_height() / 2);
			bottom = top + scaled_size.get_height();
		}

		return rect(point(left, top), point(right, bottom));
	}

	rect window_layout::build_area(const window& parent_window) const {
		return build_area(parent_window.get_area(), parent_window.get_area_scale());
	}

	void window_layout::read_from_archive(archive_reader& reader) {
		read_object(reader, "anchors", _anchors);
		read_object(reader, "margins", _margins);
		read_size(reader, "size", _size);
	}

	void window_layout::write_to_archive(archive_writer& writer) const {
		write_object(writer, "anchors", _anchors);
		write_object(writer, "margins", _margins);
		write_size(writer, "size", _size);
	}

}
