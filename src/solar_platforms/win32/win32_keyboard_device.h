#pragma once

#include <vector>
#include "solar/input/keyboard_device.h"
#include "win32_headers.h"

namespace solar {

	class win32_keyboard_device : public keyboard_device {
	public:
		static bool is_virtual_key_down(int vk);

	private:
		std::vector<keyboard_device_event_handler*> _event_handlers;

	public:
		win32_keyboard_device();
		virtual ~win32_keyboard_device();

		bool handle_wnd_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

		virtual void add_event_handler(keyboard_device_event_handler* handler) override;
		virtual void remove_event_handler(keyboard_device_event_handler* handler) override;
		virtual bool is_alt_down() const override;
		virtual bool is_ctrl_down() const override;
		virtual bool is_shift_down() const override;

	private:
		keyboard_key get_keyboard_key_from_wparam(WPARAM wparam) const;
		bool on_key_down(WPARAM wparam, LPARAM lparam);
		bool on_key_up(WPARAM wparam, LPARAM lparam);
		bool on_char_received(WPARAM wparam, LPARAM lparam);
	};

}
