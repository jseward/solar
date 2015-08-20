#include "input_mapping.h"

#include "solar/utility/assert.h"

namespace solar {

	input_mapping::input_mapping() 
		: _modifiers(input_mapping_modifiers::NONE)
		, _binding(input_mapping_binding::invalid)
		, _keyboard_key(keyboard_key::invalid)
		, _mouse_button(mouse_button::invalid) {
	}

	bool input_mapping::is_equal_ignore_modifiers(const input_mapping& lhs, const input_mapping& rhs) {
		return
			lhs._binding == rhs._binding &&
			lhs._keyboard_key == rhs._keyboard_key &&
			lhs._mouse_button == rhs._mouse_button;
	}

	bool input_mapping::operator==(const input_mapping& rhs) const {
		return
			is_equal_ignore_modifiers(*this, rhs) &&
			this->_modifiers == rhs._modifiers;
	}

	bool input_mapping::operator!=(const input_mapping& rhs) const {
		return !(*this == rhs);
	}

	input_mapping& input_mapping::set_keyboard_key(keyboard_key key) {
		_binding = input_mapping_binding::KEYBOARD_KEY;
		_keyboard_key = key;
		return *this;
	}

	input_mapping& input_mapping::set_mouse_button(mouse_button button) {
		_binding = input_mapping_binding::MOUSE_BUTTON;
		_mouse_button = button;
		return *this;
	}

	input_mapping& input_mapping::set_modifiers(input_mapping_modifiers modifiers) {
		_modifiers = modifiers;
		return *this;
	}

	input_mapping_modifiers input_mapping::get_modifiers() const {
		return _modifiers;
	}

	input_mapping_binding input_mapping::get_binding() const {
		return _binding;
	}

	keyboard_key input_mapping::get_keyboard_key() const {
		ASSERT(_binding == input_mapping_binding::KEYBOARD_KEY);
		return _keyboard_key;
	}

	mouse_button input_mapping::get_mouse_button() const {
		ASSERT(_binding == input_mapping_binding::MOUSE_BUTTON);
		return _mouse_button;
	}

}