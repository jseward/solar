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

	class font_alignment_details {
	public:
		typedef font_alignment enum_type;

		static const char* get_string(font_alignment e);
		static bool is_top(font_alignment e);
		static bool is_right(font_alignment e);
		static bool is_bottom(font_alignment e);
		static bool is_center_horizontal(font_alignment e);
		static bool is_center_vertical(font_alignment e);
	};

}