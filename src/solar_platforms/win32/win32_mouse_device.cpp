#include "win32_mouse_device.h"

#include "solar/utility/alert.h"
#include "solar/utility/assert.h"
#include "solar/utility/verify.h"
#include "solar/utility/trace.h"
#include "solar/containers/container_helpers.h"
#include "solar/utility/unused_parameter.h"
#include "win32_keyboard_device.h" //for is_virtual_key_down

namespace solar {

	win32_mouse_device::win32_mouse_device() {
	}

	win32_mouse_device::~win32_mouse_device() {
		ASSERT(_event_handlers.empty());
	}

	void win32_mouse_device::setup(HWND hwnd) {

		ULONG_PTR style = ::GetClassLongPtr(hwnd, GCL_STYLE);
		ASSERT((style & CS_DBLCLKS) != 0); //need double clicks!

		//Taking Advantage of High-Definition Mouse Movement: http://msdn.microsoft.com/en-us/library/bb206183(VS.85).aspx
		RAWINPUTDEVICE rid[1];
		rid[0].usUsagePage = ((USHORT)0x01);
		rid[0].usUsage = ((USHORT)0x02);
		rid[0].dwFlags = RIDEV_INPUTSINK;
		rid[0].hwndTarget = hwnd;
		VERIFY(::RegisterRawInputDevices(rid, 1, sizeof(rid[0])) == TRUE);

	}

	bool win32_mouse_device::handle_wnd_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
		UNUSED_PARAMETER(hwnd);

		bool handled = false;
		
		switch (message) {
			case WM_LBUTTONDOWN: {
				on_mouse_button_down(mouse_button::LEFT);
				handled = true;
				break;
			};

			case WM_LBUTTONUP: {
				on_mouse_button_up(mouse_button::LEFT);
				handled = true;
				break;
			}

			case WM_LBUTTONDBLCLK: {
				on_mouse_button_double_clicked(mouse_button::LEFT);
				handled = true;
				break;
			}

			case WM_MBUTTONDOWN: {
				on_mouse_button_down(mouse_button::MIDDLE);
				handled = true;
				break;
			};

			case WM_MBUTTONUP: {
				on_mouse_button_up(mouse_button::MIDDLE);
				handled = true;
				break;
			}

			case WM_MBUTTONDBLCLK: {
				on_mouse_button_double_clicked(mouse_button::MIDDLE);
				handled = true;
				break;
			}

			case WM_RBUTTONDOWN: {
				on_mouse_button_down(mouse_button::RIGHT);
				handled = true;
				break;
			};

			case WM_RBUTTONUP: {
				on_mouse_button_up(mouse_button::RIGHT);
				handled = true;
				break;
			}

			case WM_RBUTTONDBLCLK: {
				on_mouse_button_double_clicked(mouse_button::RIGHT);
				handled = true;
				break;
			}

			case WM_XBUTTONDOWN: {
				on_mouse_button_down(get_extra_mouse_button_from_wparam(wparam));
				handled = true;
				break;
			}

			case WM_XBUTTONUP: {
				on_mouse_button_up(get_extra_mouse_button_from_wparam(wparam));
				handled = true;
				break;
			}

			case WM_XBUTTONDBLCLK: {
				on_mouse_button_double_clicked(get_extra_mouse_button_from_wparam(wparam));
				handled = true;
				break;
			}

			case WM_MOUSEWHEEL: {
				handled = on_mouse_wheel(wparam, lparam);
				break;
			}

			case WM_MOUSEMOVE: {
				handled = on_mouse_move(wparam, lparam);
				break;
			}

			case WM_INPUT: {
				handled = on_wm_input(wparam, lparam);
				break;
			}
		}

		return handled;
	}

	void win32_mouse_device::add_event_handler(mouse_device_event_handler* handler) {
		push_back_and_verify_not_found(_event_handlers, handler);
	}

	void win32_mouse_device::remove_event_handler(mouse_device_event_handler* handler) {
		find_and_erase(_event_handlers, handler);
	}

	bool win32_mouse_device::is_mouse_button_down(mouse_button button) const {
		int vk = 0;
		switch (button) {
			case mouse_button::LEFT: vk = VK_LBUTTON; break;
			case mouse_button::MIDDLE: vk = VK_MBUTTON; break;
			case mouse_button::RIGHT: vk = VK_RBUTTON; break;
			case mouse_button::EXTRA_0: vk = VK_XBUTTON1; break;
			case mouse_button::EXTRA_1: vk = VK_XBUTTON2; break;
			default: ALERT("unsupported mouse_button : {}", to_string(button));
		}
		
		if (vk != 0) {
			return win32_keyboard_device::is_virtual_key_down(vk);
		}
		return false;
	}

	mouse_button win32_mouse_device::get_extra_mouse_button_from_wparam(WPARAM wparam) const {
		if (GET_XBUTTON_WPARAM(wparam) == XBUTTON1) {
			return mouse_button::EXTRA_0;
		}
		else {
			ASSERT(GET_XBUTTON_WPARAM(wparam) == XBUTTON2);
			return mouse_button::EXTRA_1;
		}
	}

	void win32_mouse_device::on_mouse_button_down(mouse_button mouse_button) {
		for (auto event_handler : _event_handlers) {
			event_handler->on_mouse_button_down(mouse_button);
		}
	}

	void win32_mouse_device::on_mouse_button_up(mouse_button mouse_button) {
		for (auto event_handler : _event_handlers) {
			event_handler->on_mouse_button_up(mouse_button);
		}
	}

	void win32_mouse_device::on_mouse_button_double_clicked(mouse_button mouse_button) {
		for (auto event_handler : _event_handlers) {
			event_handler->on_mouse_button_double_clicked(mouse_button);
		}
	}

	bool win32_mouse_device::on_mouse_move(WPARAM wparam, LPARAM lparam) {
		UNUSED_PARAMETER(wparam);

		for (auto event_handler : _event_handlers) {
			event_handler->on_mouse_moved(
				GET_X_LPARAM(lparam), 
				GET_Y_LPARAM(lparam));
		}
		return true;
	}

	bool win32_mouse_device::on_wm_input(WPARAM wparam, LPARAM lparam) {
		UNUSED_PARAMETER(wparam);

		//Taking Advantage of High-Definition Mouse Movement
		//http://msdn.microsoft.com/en-us/library/bb206183(VS.85).aspx
		#if defined(WIN64)
			UINT data_size = 48;
			BYTE data[48];
		#elif defined(WIN32)
			UINT data_size = 40;
			BYTE data[40];
		#else
			#error No operating system for raw input
		#endif

		::GetRawInputData((HRAWINPUT)lparam, RID_INPUT, data, &data_size, sizeof(RAWINPUTHEADER));

		const RAWINPUT* raw = (RAWINPUT*)data;
		IF_VERIFY(raw->header.dwType == RIM_TYPEMOUSE) {
			for (auto event_handler : _event_handlers) {
				event_handler->on_mouse_moved_high_res(
					raw->data.mouse.lLastX, 
					raw->data.mouse.lLastY);
			}
			return true;
		}

		return false;
	}

	bool win32_mouse_device::on_mouse_wheel(WPARAM wparam, LPARAM lparam) {
		UNUSED_PARAMETER(lparam);

		//http://msdn.microsoft.com/en-us/library/windows/desktop/ms645617(v=vs.85).aspx
		//The wheel rotation will be a multiple of WHEEL_DELTA, which is set at 120. This is the threshold for action to be taken, and one such action(for example, scrolling one increment) should occur for each delta.
		float delta = GET_WHEEL_DELTA_WPARAM(wparam) / 120.f;
		for (auto event_handler : _event_handlers) {
			event_handler->on_mouse_wheel_moved(delta);
		}
		return true;
	}


}