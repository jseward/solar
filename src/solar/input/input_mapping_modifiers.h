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

	extern bool input_mapping_modifiers_has_alt(input_mapping_modifiers modifiers);
	extern bool input_mapping_modifiers_has_ctrl(input_mapping_modifiers modifiers);
	extern bool input_mapping_modifiers_has_shift(input_mapping_modifiers modifiers);
	extern bool input_mapping_modifiers_is_subset_of(input_mapping_modifiers check_against, input_mapping_modifiers check_with);
	extern input_mapping_modifiers make_input_mapping_modifiers(bool ctrl, bool shift, bool alt);

}