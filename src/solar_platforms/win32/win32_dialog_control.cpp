#include "win32_dialog_control.h"

#include "solar/utility/assert.h"
#include "solar/utility/unused_parameter.h"

namespace solar {

	win32_dialog_control::win32_dialog_control(win32_dialog_control_anchor anchor, int resource_id) 
		: _anchor(anchor)
		, _resource_id(resource_id) {
	}

	void win32_dialog_control::on_wm_initdialog(HWND hdlg, WPARAM wparam, LPARAM lparam) {
		UNUSED_PARAMETER(lparam);
		UNUSED_PARAMETER(wparam);

		RECT item_rect_in_screen_space = { 0 };
		::GetWindowRect(::GetDlgItem(hdlg, _resource_id), &item_rect_in_screen_space);

		POINT item_top_left_in_dlg_space = { 0 };
		item_top_left_in_dlg_space.x = item_rect_in_screen_space.left;
		item_top_left_in_dlg_space.y = item_rect_in_screen_space.top;
		::ScreenToClient(hdlg, &item_top_left_in_dlg_space);

		POINT item_bottom_right_in_dlg_space = { 0 };
		item_bottom_right_in_dlg_space.x = item_rect_in_screen_space.right;
		item_bottom_right_in_dlg_space.y = item_rect_in_screen_space.bottom;
		::ScreenToClient(hdlg, &item_bottom_right_in_dlg_space);

		_initial_area_in_dlg_space = rect(
			point(item_top_left_in_dlg_space.x, item_top_left_in_dlg_space.y),
			point(item_bottom_right_in_dlg_space.x, item_bottom_right_in_dlg_space.y));

		RECT dlg_rect = { 0 };
		::GetClientRect(hdlg, &dlg_rect);
		ASSERT(dlg_rect.left == 0);
		ASSERT(dlg_rect.top == 0);
		_initial_dlg_size = size(dlg_rect.right, dlg_rect.bottom);
	}

	void win32_dialog_control::on_wm_size(HWND hdlg, WPARAM wparam, LPARAM lparam) {
		UNUSED_PARAMETER(wparam);

		size new_dlg_size = size(LOWORD(lparam), HIWORD(lparam));

		if (_anchor == win32_dialog_control_anchor::ALL) {
			::MoveWindow(
				::GetDlgItem(hdlg, _resource_id),
				_initial_area_in_dlg_space.get_left(),
				_initial_area_in_dlg_space.get_top(),
				new_dlg_size._width - (_initial_dlg_size._width - _initial_area_in_dlg_space.get_width()),
				new_dlg_size._height - (_initial_dlg_size._height - _initial_area_in_dlg_space.get_height()),
				FALSE);
		}
		else if (_anchor == win32_dialog_control_anchor::BOTTOM_RIGHT) {
			::MoveWindow(
				::GetDlgItem(hdlg, _resource_id),
				new_dlg_size._width - (_initial_dlg_size._width - _initial_area_in_dlg_space.get_left()),
				new_dlg_size._height - (_initial_dlg_size._height - _initial_area_in_dlg_space.get_top()),
				_initial_area_in_dlg_space.get_width(),
				_initial_area_in_dlg_space.get_height(),
				FALSE);
		}
		else {
			ASSERT(false);
		}
	}

}