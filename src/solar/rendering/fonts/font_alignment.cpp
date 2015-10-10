#include "font_alignment.h"
#include "solar/utility/enum_helpers.h"

namespace solar {

	const char* font_alignment_details::get_string(font_alignment e) {
		switch (e) {
			RETURN_ENUM_STRING(font_alignment, LEFT);
			RETURN_ENUM_STRING(font_alignment, CENTER);
			RETURN_ENUM_STRING(font_alignment, RIGHT);
			RETURN_ENUM_STRING(font_alignment, TOP_LEFT);
			RETURN_ENUM_STRING(font_alignment, TOP_CENTER);
			RETURN_ENUM_STRING(font_alignment, TOP_RIGHT);
			RETURN_ENUM_STRING(font_alignment, BOTTOM_LEFT);
			RETURN_ENUM_STRING(font_alignment, BOTTOM_CENTER);
			RETURN_ENUM_STRING(font_alignment, BOTTOM_RIGHT);
			END_ENUM_STRINGS(e);
		}
	}

	bool font_alignment_details::is_top(font_alignment e) {
		switch (e) {
		case font_alignment::TOP_LEFT:
		case font_alignment::TOP_CENTER:
		case font_alignment::TOP_RIGHT:
			return true;

		case font_alignment::LEFT:
		case font_alignment::CENTER:
		case font_alignment::RIGHT:
		case font_alignment::BOTTOM_LEFT:
		case font_alignment::BOTTOM_CENTER:
		case font_alignment::BOTTOM_RIGHT:
		default:
			return false;
		}
	}

	bool font_alignment_details::is_right(font_alignment e) {
		switch (e) {
		case font_alignment::RIGHT:
		case font_alignment::TOP_RIGHT:
		case font_alignment::BOTTOM_RIGHT:
			return true;

		case font_alignment::LEFT:
		case font_alignment::CENTER:
		case font_alignment::TOP_LEFT:
		case font_alignment::TOP_CENTER:
		case font_alignment::BOTTOM_LEFT:
		case font_alignment::BOTTOM_CENTER:
		default:
			return false;
		}
	}

	bool font_alignment_details::is_bottom(font_alignment e) {
		switch (e) {
		case font_alignment::BOTTOM_LEFT:
		case font_alignment::BOTTOM_CENTER:
		case font_alignment::BOTTOM_RIGHT:
			return true;

		case font_alignment::LEFT:
		case font_alignment::CENTER:
		case font_alignment::RIGHT:
		case font_alignment::TOP_LEFT:
		case font_alignment::TOP_CENTER:
		case font_alignment::TOP_RIGHT:
		default:
			return false;
		}
	}

	bool font_alignment_details::is_center_horizontal(font_alignment e) {
		switch (e) {
		case font_alignment::TOP_CENTER:
		case font_alignment::CENTER:
		case font_alignment::BOTTOM_CENTER:
			return true;

		case font_alignment::LEFT:
		case font_alignment::RIGHT:
		case font_alignment::TOP_LEFT:
		case font_alignment::TOP_RIGHT:
		case font_alignment::BOTTOM_LEFT:
		case font_alignment::BOTTOM_RIGHT:
		default:
			return false;
		}
	}

	bool font_alignment_details::is_center_vertical(font_alignment e) {
		switch (e) {
		case font_alignment::LEFT:
		case font_alignment::CENTER:
		case font_alignment::RIGHT:
			return true;

		case font_alignment::TOP_LEFT:
		case font_alignment::TOP_CENTER:
		case font_alignment::TOP_RIGHT:
		case font_alignment::BOTTOM_LEFT:
		case font_alignment::BOTTOM_CENTER:
		case font_alignment::BOTTOM_RIGHT:
		default:
			return false;
		}
	}

}