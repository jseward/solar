#pragma once

#include "input_mapping_modifiers.h"
#include "input_mapping_binding.h"
#include "keyboard_key.h"
#include "mouse_button.h"

namespace solar {

	class input_mapping {
	private:
		input_mapping_modifiers _modifiers;
		input_mapping_binding _binding;
		keyboard_key _keyboard_key;
		mouse_button _mouse_button;

	public:
		input_mapping();

		bool operator==(const input_mapping& rhs) const;
		bool operator!=(const input_mapping& rhs) const;

		input_mapping& set_keyboard_key(keyboard_key key);
		input_mapping& set_mouse_button(mouse_button button);
		input_mapping& set_modifiers(input_mapping_modifiers modifiers);

		input_mapping_modifiers get_modifiers() const;
		input_mapping_binding get_binding() const;
		keyboard_key get_keyboard_key() const;
		mouse_button get_mouse_button() const;

	public:
		static bool is_equal_ignore_modifiers(const input_mapping& lhs, const input_mapping& rhs);
	};

}