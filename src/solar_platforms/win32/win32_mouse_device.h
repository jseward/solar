#pragma once

#include <vector>
#include "solar/input/mouse_device.h"
#include "win32_headers.h"

namespace solar {

	class win32_mouse_device : public mouse_device {
	private:
		std::vector<mouse_device_event_handler*> _event_handlers;

	public:
		win32_mouse_device();
		virtual ~win32_mouse_device();

		void setup(HWND hwnd);

		bool handle_wnd_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

		virtual void add_event_handler(mouse_device_event_handler* handler) override;
		virtual void remove_event_handler(mouse_device_event_handler* handler) override;
		virtual bool is_mouse_button_down(mouse_button button) const override;

	private:
		mouse_button get_extra_mouse_button_from_wparam(WPARAM wparam) const;
		void on_mouse_button_down(mouse_button mouse_button);
		void on_mouse_button_up(mouse_button mouse_button);
		void on_mouse_button_double_clicked(mouse_button mouse_button);
		bool on_mouse_move(WPARAM wparam, LPARAM lparam);
		bool on_wm_input(WPARAM wparam, LPARAM lparam);
		bool on_mouse_wheel(WPARAM wparam, LPARAM lparam);
	};

}