#pragma once

namespace solar {

	enum class input_mapping_modifiers {
		NONE,
		ALT,
		CTRL,
		SHIFT,
		SHIFT_ALT,
		CTRL_ALT,
		CTRL_SHIFT,
		CTRL_SHIFT_ALT
	};

	struct input_mapping_modifiers_details {
		static bool has_alt(input_mapping_modifiers modifiers);
		static bool has_ctrl(input_mapping_modifiers modifiers);
		static bool has_shift(input_mapping_modifiers modifiers);
		static bool is_subset_of(input_mapping_modifiers check_against, input_mapping_modifiers check_with);
		static input_mapping_modifiers make_input_mapping_modifiers(bool ctrl, bool shift, bool alt);
	};

}