#include "win32_keyboard_device.h"

#include "solar/utility/alert.h"
#include "solar/utility/assert.h"
#include "solar/utility/verify.h"
#include "solar/utility/trace.h"
#include "solar/containers/container_helpers.h"
#include "solar/utility/unused_parameter.h"

namespace solar {

	bool win32_keyboard_device::is_virtual_key_down(int vk) {
		return (::GetAsyncKeyState(vk) & 0x8000) != 0;
	}

	win32_keyboard_device::win32_keyboard_device() {
	}

	win32_keyboard_device::~win32_keyboard_device() {
		ASSERT(_event_handlers.empty());
	}

	bool win32_keyboard_device::handle_wnd_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
		UNUSED_PARAMETER(hwnd);

		bool handled = false;
		
		switch (message) {
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN: {
				handled = on_key_down(wparam, lparam);
				break;
			}

			case WM_KEYUP:
			case WM_SYSKEYUP: {
				handled = on_key_up(wparam, lparam);
				break;
			}

			case WM_CHAR:
			case WM_SYSCHAR: {
				handled = on_char_received(wparam, lparam);
				break;
			}
		}

		return handled;
	}

	void win32_keyboard_device::add_event_handler(keyboard_device_event_handler* handler) {
		push_back_and_verify_not_found(_event_handlers, handler);
	}

	void win32_keyboard_device::remove_event_handler(keyboard_device_event_handler* handler) {
		find_and_erase(_event_handlers, handler);
	}

	bool win32_keyboard_device::is_alt_down() const {
		return is_virtual_key_down(VK_MENU);
	}

	bool win32_keyboard_device::is_ctrl_down() const {
		return is_virtual_key_down(VK_CONTROL);
	}

	bool win32_keyboard_device::is_shift_down() const {
		return is_virtual_key_down(VK_SHIFT);
	}

	bool win32_keyboard_device::on_key_down(WPARAM wparam, LPARAM lparam) {
		keyboard_key key = get_keyboard_key_from_wparam(wparam);
		if (key != keyboard_key::invalid) {
			//http://stackoverflow.com/questions/6176566/get-30th-bit-of-the-lparam-param-in-wm-keydown-message
			bool was_down = ((lparam & 0x40000000) != 0);
			if (!was_down) {
				for (auto event_handler : _event_handlers) {
					event_handler->on_key_down(key);
				}
			}
			return true;
		}
		return false;
	}

	bool win32_keyboard_device::on_key_up(WPARAM wparam, LPARAM lparam) {
		UNUSED_PARAMETER(lparam);

		keyboard_key key = get_keyboard_key_from_wparam(wparam);
		if (key != keyboard_key::invalid) {
			for (auto event_handler : _event_handlers) {
				event_handler->on_key_up(key);
			}
			return true;
		}
		return false;
	}

	bool win32_keyboard_device::on_char_received(WPARAM wparam, LPARAM lparam) {
		UNUSED_PARAMETER(lparam);

		wchar_t char_code = (wchar_t)wparam;
		if (char_code >= L' ') {
			for (auto event_handler : _event_handlers) {
				event_handler->on_char_received(char_code);
			}
			return true;
		}
		return false;
	}

	keyboard_key win32_keyboard_device::get_keyboard_key_from_wparam(WPARAM wparam) const {
		switch (wparam) {
			case VK_BACK: return keyboard_key::BACKSPACE;
			case VK_TAB: return keyboard_key::TAB;
			case VK_RETURN: return keyboard_key::ENTER;
			case VK_SHIFT: return keyboard_key::SHIFT;
			case VK_CONTROL: return keyboard_key::CTRL;
			case VK_PAUSE: return keyboard_key::PAUSE;
			case VK_ESCAPE: return keyboard_key::ESCAPE;
			case VK_SPACE: return keyboard_key::SPACE;
			case VK_PRIOR: return keyboard_key::PAGE_UP;
			case VK_NEXT: return keyboard_key::PAGE_DOWN;
			case VK_END: return keyboard_key::END;
			case VK_HOME: return keyboard_key::HOME;
			case VK_UP: return keyboard_key::UP;
			case VK_DOWN: return keyboard_key::DOWN;
			case VK_LEFT: return keyboard_key::LEFT;
			case VK_RIGHT: return keyboard_key::RIGHT;
			case VK_INSERT: return keyboard_key::INSERT;
			case VK_DELETE: return keyboard_key::DEL;
			case VK_HELP: return keyboard_key::HELP;
			case VK_OEM_1: return keyboard_key::SEMI_COLON;
			case VK_OEM_PLUS: return keyboard_key::PLUS;
			case VK_OEM_COMMA: return keyboard_key::COMMA;
			case VK_OEM_MINUS: return keyboard_key::MINUS;
			case VK_OEM_PERIOD: return keyboard_key::PERIOD;
			case VK_OEM_2: return keyboard_key::FORWARD_SLASH;
			case VK_OEM_3: return keyboard_key::TILDE;
			case VK_OEM_4: return keyboard_key::OPEN_BRACKET;
			case VK_OEM_5: return keyboard_key::BACK_SLASH;
			case VK_OEM_6: return keyboard_key::CLOSE_BRACKET;
			case VK_OEM_7: return keyboard_key::QUOTE;
			case VK_SNAPSHOT: return keyboard_key::PRINT_SCREEN;
			case VK_SCROLL: return keyboard_key::SCROLL_LOCK;
			case VK_F1: return keyboard_key::F1;
			case VK_F2: return keyboard_key::F2;
			case VK_F3: return keyboard_key::F3;
			case VK_F4: return keyboard_key::F4;
			case VK_F5: return keyboard_key::F5;
			case VK_F6: return keyboard_key::F6;
			case VK_F7: return keyboard_key::F7;
			case VK_F8: return keyboard_key::F8;
			case VK_F9: return keyboard_key::F9;
			case VK_F10: return keyboard_key::F10;
			case VK_F11: return keyboard_key::F11;
			case VK_F12: return keyboard_key::F12;
			case '0': return keyboard_key::N0;
			case '1': return keyboard_key::N1;
			case '2': return keyboard_key::N2;
			case '3': return keyboard_key::N3;
			case '4': return keyboard_key::N4;
			case '5': return keyboard_key::N5;
			case '6': return keyboard_key::N6;
			case '7': return keyboard_key::N7;
			case '8': return keyboard_key::N8;
			case '9': return keyboard_key::N9;
			case 'A': return keyboard_key::A;
			case 'B': return keyboard_key::B;
			case 'C': return keyboard_key::C;
			case 'D': return keyboard_key::D;
			case 'E': return keyboard_key::E;
			case 'F': return keyboard_key::F;
			case 'G': return keyboard_key::G;
			case 'H': return keyboard_key::H;
			case 'I': return keyboard_key::I;
			case 'J': return keyboard_key::J;
			case 'K': return keyboard_key::K;
			case 'L': return keyboard_key::L;
			case 'M': return keyboard_key::M;
			case 'N': return keyboard_key::N;
			case 'O': return keyboard_key::O;
			case 'P': return keyboard_key::P;
			case 'Q': return keyboard_key::Q;
			case 'R': return keyboard_key::R;
			case 'S': return keyboard_key::S;
			case 'T': return keyboard_key::T;
			case 'U': return keyboard_key::U;
			case 'V': return keyboard_key::V;
			case 'W': return keyboard_key::W;
			case 'X': return keyboard_key::X;
			case 'Y': return keyboard_key::Y;
			case 'Z': return keyboard_key::Z;
			default: break;
		}

		return keyboard_key::invalid;
	}

}