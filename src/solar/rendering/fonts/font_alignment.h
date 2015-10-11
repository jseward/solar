#pragma once

namespace solar {

	enum class font_alignment {
		invalid = -1,

		LEFT,
		CENTER,
		RIGHT,
		TOP_LEFT,
		TOP_CENTER,
		TOP_RIGHT,
		BOTTOM_LEFT,
		BOTTOM_CENTER,
		BOTTOM_RIGHT,

		count
	};

	extern const char* to_string(font_alignment e);
	extern bool font_alignment_is_top(font_alignment e);
	extern bool font_alignment_is_right(font_alignment e);
	extern bool font_alignment_is_bottom(font_alignment e);
	extern bool font_alignment_is_center_horizontal(font_alignment e);
	extern bool font_alignment_is_center_vertical(font_alignment e);

}