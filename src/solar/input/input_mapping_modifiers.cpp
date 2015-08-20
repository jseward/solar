#include "input_mapping_modifiers.h"

namespace solar {

	bool input_mapping_modifiers_details::has_alt(input_mapping_modifiers modifiers) {
		return
			(modifiers == input_mapping_modifiers::ALT) ||
			(modifiers == input_mapping_modifiers::CTRL_ALT) ||
			(modifiers == input_mapping_modifiers::SHIFT_ALT) ||
			(modifiers == input_mapping_modifiers::CTRL_SHIFT_ALT);
	}

	bool input_mapping_modifiers_details::has_ctrl(input_mapping_modifiers modifiers) {
		return
			(modifiers == input_mapping_modifiers::CTRL) ||
			(modifiers == input_mapping_modifiers::CTRL_ALT) ||
			(modifiers == input_mapping_modifiers::CTRL_SHIFT) ||
			(modifiers == input_mapping_modifiers::CTRL_SHIFT_ALT);
	}

	bool input_mapping_modifiers_details::has_shift(input_mapping_modifiers modifiers) {
		return
			(modifiers == input_mapping_modifiers::SHIFT) ||
			(modifiers == input_mapping_modifiers::SHIFT_ALT) ||
			(modifiers == input_mapping_modifiers::CTRL_SHIFT) ||
			(modifiers == input_mapping_modifiers::CTRL_SHIFT_ALT);
	}

	bool input_mapping_modifiers_details::is_subset_of(input_mapping_modifiers check_against, input_mapping_modifiers check_with) {
		if (check_against == input_mapping_modifiers::NONE) {
			return false;
		}

		if (has_alt(check_against) && !has_alt(check_with)) {
			return false;
		}

		if (has_ctrl(check_against) && !has_ctrl(check_with)) {
			return false;
		}

		if (has_shift(check_against) && !has_shift(check_with)) {
			return false;
		}

		return true;
	}

	input_mapping_modifiers input_mapping_modifiers_details::make_input_mapping_modifiers(bool ctrl, bool shift, bool alt) {
		if (ctrl) {
			if (shift) {
				if (alt) {
					return input_mapping_modifiers::CTRL_SHIFT_ALT;
				}
				else {
					return input_mapping_modifiers::CTRL_SHIFT;
				}
			}
			else {
				if (alt) {
					return input_mapping_modifiers::CTRL_ALT;
				}
				else {
					return input_mapping_modifiers::CTRL;
				}
			}
		}
		else {
			if (shift) {
				if (alt) {
					return input_mapping_modifiers::SHIFT_ALT;
				}
				else {
					return input_mapping_modifiers::SHIFT;
				}
			}
			else {
				if (alt) {
					return input_mapping_modifiers::ALT;
				}
				else {
					return input_mapping_modifiers::NONE;
				}
			}
		}
	}

}
