#include "brush_stretch_margins.h"
#include "solar/archiving/archiving_helpers.h"
#include "solar/utility/assert.h"
#include "solar/utility/type_convert.h"

namespace solar {

	brush_stretch_margins::brush_stretch_margins() 
		: _left(0)
		, _top(0)
		, _right(0)
		, _bottom(0) {
	}

	brush_stretch_margins::brush_stretch_margins(int left, int top, int right, int bottom)
		: _left(left)
		, _top(top)
		, _right(right)
		, _bottom(bottom) {
	}

	bool brush_stretch_margins::has_any() const {
		return
			_left != 0 ||
			_top != 0 ||
			_right != 0 ||
			_bottom != 0;
	}

	bool brush_stretch_margins::has_left() const {
		return _left != 0;
	}

	bool brush_stretch_margins::has_top() const {
		return _top != 0;
	}

	bool brush_stretch_margins::has_right() const {
		return _right != 0;
	}

	bool brush_stretch_margins::has_bottom() const {
		return _bottom != 0;
	}

	enum_vector<brush_stretch_region, brush_stretch_region> brush_stretch_margins::get_all_non_empty_stretch_regions() const {
		enum_vector<brush_stretch_region, brush_stretch_region> regions;

		if (has_left() && has_top()) {
			regions.push_back(brush_stretch_region::TOP_LEFT);
		}

		if (has_top()) {
			regions.push_back(brush_stretch_region::TOP);
		}

		if (has_right() && has_top()) {
			regions.push_back(brush_stretch_region::TOP_RIGHT);
		}

		if (has_right()) {
			regions.push_back(brush_stretch_region::RIGHT);
		}

		if (has_right() && has_bottom()) {
			regions.push_back(brush_stretch_region::BOTTOM_RIGHT);
		}

		if (has_bottom()) {
			regions.push_back(brush_stretch_region::BOTTOM);
		}

		if (has_left() && has_bottom()) {
			regions.push_back(brush_stretch_region::BOTTOM_LEFT);
		}

		if (has_left()) {
			regions.push_back(brush_stretch_region::LEFT);
		}

		//if no edge regions exist don't add the center region. there should be no stretch regions at all so clients can handle that situation specially.
		if (!regions.empty()) {
			regions.push_back(brush_stretch_region::CENTER);
		}

		return regions;
	}

	bool brush_stretch_margins::can_build_region_rect(const rect& full_rect) const {
		return
			(_left + _right) <= full_rect.get_width() &&
			(_top + _bottom) <= full_rect.get_height();
	}

	rect brush_stretch_margins::build_region_rect(const rect& full_rect, brush_stretch_region region) const {
		ASSERT(can_build_region_rect(full_rect));

		switch (region) {
		case brush_stretch_region::CENTER:
			return rect(
				full_rect.get_left() + _left,
				full_rect.get_top() + _top,
				full_rect.get_right() - _right,
				full_rect.get_bottom() - _bottom);
			break;

		case brush_stretch_region::TOP_LEFT:
			return rect(
				full_rect.get_left(),
				full_rect.get_top(),
				full_rect.get_left() + _left,
				full_rect.get_top() + _top);
			break;

		case brush_stretch_region::TOP:
			return rect(
				full_rect.get_left() + _left,
				full_rect.get_top(),
				full_rect.get_right() - _right,
				full_rect.get_top() + _top);
			break;

		case brush_stretch_region::TOP_RIGHT:
			return rect(
				full_rect.get_right() - _right,
				full_rect.get_top(),
				full_rect.get_right(),
				full_rect.get_top() + _top);
			break;

		case brush_stretch_region::RIGHT:
			return rect(
				full_rect.get_right() - _right,
				full_rect.get_top() + _top,
				full_rect.get_right(),
				full_rect.get_bottom() - _bottom);
			break;

		case brush_stretch_region::BOTTOM_RIGHT:
			return rect(
				full_rect.get_right() - _right,
				full_rect.get_bottom() - _bottom,
				full_rect.get_right(),
				full_rect.get_bottom());
			break;

		case brush_stretch_region::BOTTOM:
			return rect(
				full_rect.get_left() + _left,
				full_rect.get_bottom() - _bottom,
				full_rect.get_right() - _right,
				full_rect.get_bottom());
			break;

		case brush_stretch_region::BOTTOM_LEFT:
			return rect(
				full_rect.get_left(),
				full_rect.get_bottom() - _bottom,
				full_rect.get_left() + _left,
				full_rect.get_bottom());
			break;

		case brush_stretch_region::LEFT:
			return rect(
				full_rect.get_left(),
				full_rect.get_top() + _top,
				full_rect.get_left() + _left,
				full_rect.get_bottom() - _bottom);
			break;

		case brush_stretch_region::invalid:
		case brush_stretch_region::count:
		default: ASSERT(false); break;
		}
		return rect();
	}

	int brush_stretch_margins::get_left() const {
		return _left;
	}

	int brush_stretch_margins::get_top() const {
		return _top;
	}

	int brush_stretch_margins::get_right() const {
		return _right;
	}

	int brush_stretch_margins::get_bottom() const {
		return _bottom;
	}

	void brush_stretch_margins::read_from_archive(archive_reader& reader, const char* name) {
		read_ints(reader, name, _left, _top, _right, _bottom);
	}

	void brush_stretch_margins::write_to_archive(archive_writer& writer, const char* name) const {
		write_ints(writer, name, _left, _top, _right, _bottom);
	}

	std::ostream& operator<<(std::ostream& os, const brush_stretch_margins& margins) {
		return os << "[" << margins.get_left() << ", " << margins.get_top() << ", " << margins.get_right() << ", " << margins.get_bottom() << "]";
	}

	brush_stretch_margins make_scaled_brush_stretch_margins(const brush_stretch_margins& unscaled_margins, float scale) {
		ASSERT(scale > 0.f);

		return brush_stretch_margins(
			float_to_int(unscaled_margins._left * scale),
			float_to_int(unscaled_margins._top * scale),
			float_to_int(unscaled_margins._right * scale),
			float_to_int(unscaled_margins._bottom * scale));
	}
}
