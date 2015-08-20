#include "win32_error_dialog.h"

#include "solar/utility/assert.h"
#include "solar/utility/unused_parameter.h"
#include "solar/strings/string_marshal.h"
#include "solar/strings/string_build.h"
#include "solar/strings/string_helpers.h"

namespace solar {

	win32_error_dialog* win32_error_dialog::s_dlg_proc_context = nullptr;

	win32_error_dialog::win32_error_dialog() 
		: _hwnd(nullptr)
		, _hinstance(nullptr)
		, _main_thread_id(0)
		, _is_first_init_dialog(true) {
	}

	void win32_error_dialog::setup(const win32_error_dialog_resources& resources, HWND hwnd, HINSTANCE hinstance) {
		_resources = resources;
		_hwnd = hwnd;
		_hinstance = hinstance;
		_main_thread_id = ::GetCurrentThreadId();

		_textbox = std::make_unique<win32_dialog_control>(win32_dialog_control_anchor::ALL, resources.get_textbox_id());
		_break_button = std::make_unique<win32_dialog_control>(win32_dialog_control_anchor::BOTTOM_RIGHT, resources.get_break_button_id());
		_skip_button = std::make_unique<win32_dialog_control>(win32_dialog_control_anchor::BOTTOM_RIGHT, resources.get_skip_button_id());
		_skip_all_button_button = std::make_unique<win32_dialog_control>(win32_dialog_control_anchor::BOTTOM_RIGHT, resources.get_skip_all_button_id());

		ASSERT(s_dlg_proc_context == nullptr);
		s_dlg_proc_context = this;
	}

	win32_error_dialog_result win32_error_dialog::show(const char* title, const char* message) {
		if (_main_thread_id == ::GetCurrentThreadId()) {
			_title = title;
			_message = message;
			string_replace_all(_message, "\n", "\r\n"); //Multi-Line text boxes don't handle new lines properly.

			if (_resources.get_dialog_id() == 0) {
				int mb_result = ::MessageBox(_hwnd, utf8_to_utf16(_message).c_str(), utf8_to_utf16(_title).c_str(), MB_ABORTRETRYIGNORE | MB_ICONERROR);
				if (mb_result == IDABORT) {
					return win32_error_dialog_result::BREAK;
				}
				else if (mb_result == IDRETRY) {
					return win32_error_dialog_result::SKIP;
				}
				else {
					return win32_error_dialog_result::SKIP_ALL;
				}
			}
			else {
				return (win32_error_dialog_result)::DialogBoxParam(
					_hinstance,
					MAKEINTRESOURCE(_resources.get_dialog_id()),
					_hwnd,
					dlg_proc,
					NULL);
			}
		}

		return win32_error_dialog_result::SKIP_ALL;
	}

	INT_PTR win32_error_dialog::dlg_proc(HWND hdlg, UINT msg, WPARAM wparam, LPARAM lparam) {
		switch (msg)
		{
		case WM_INITDIALOG:
			return s_dlg_proc_context->on_wm_initdialog(hdlg, wparam, lparam);

		case WM_COMMAND:
			return s_dlg_proc_context->on_wm_command(hdlg, wparam, lparam);

		case WM_SIZE:
			return s_dlg_proc_context->on_wm_size(hdlg, wparam, lparam);

		case WM_GETMINMAXINFO:
			return s_dlg_proc_context->on_wm_getminmaxinfo(hdlg, wparam, lparam);
		}

		return FALSE;
	}

	BOOL win32_error_dialog::on_wm_initdialog(HWND hdlg, WPARAM wparam, LPARAM lparam) {
		if (_is_first_init_dialog) {

			_textbox->on_wm_initdialog(hdlg, wparam, lparam);
			_break_button->on_wm_initdialog(hdlg, wparam, lparam);
			_skip_button->on_wm_initdialog(hdlg, wparam, lparam);
			_skip_all_button_button->on_wm_initdialog(hdlg, wparam, lparam);

			RECT dialog_rect = { 0 };
			::GetWindowRect(hdlg, &dialog_rect);
			
			_min_track_size = size(
				dialog_rect.right - dialog_rect.left,
				dialog_rect.bottom - dialog_rect.top);

			_saved_dlg_rect = dialog_rect;

			_is_first_init_dialog = false;
		}
		else {
			::MoveWindow(
				hdlg, 
				_saved_dlg_rect.left, 
				_saved_dlg_rect.top, 
				_saved_dlg_rect.right - _saved_dlg_rect.left, 
				_saved_dlg_rect.bottom - _saved_dlg_rect.top, 
				TRUE);
		}

		::SetWindowTextA(hdlg, _title.c_str());
		::SetDlgItemTextA(hdlg, _resources.get_textbox_id(), _message.c_str());

		return TRUE;
	}

	BOOL win32_error_dialog::on_wm_command(HWND hdlg, WPARAM wparam, LPARAM lparam) {
		UNUSED_PARAMETER(lparam);
		if (wparam == _resources.get_break_button_id()) {
			::EndDialog(hdlg, (int)win32_error_dialog_result::BREAK);
			return TRUE;
		}
		else if (wparam == IDCANCEL || wparam == _resources.get_skip_button_id()) {
			::EndDialog(hdlg, (int)win32_error_dialog_result::SKIP);
			return TRUE;
		}
		else if (wparam == _resources.get_skip_all_button_id()) {
			::EndDialog(hdlg, (int)win32_error_dialog_result::SKIP_ALL);
			return TRUE;
		}
		return FALSE;
	}

	BOOL win32_error_dialog::on_wm_size(HWND hdlg, WPARAM wparam, LPARAM lparam) {

		::GetWindowRect(hdlg, &_saved_dlg_rect);

		_textbox->on_wm_size(hdlg, wparam, lparam);
		_break_button->on_wm_size(hdlg, wparam, lparam);
		_skip_button->on_wm_size(hdlg, wparam, lparam);
		_skip_all_button_button->on_wm_size(hdlg, wparam, lparam);

		::InvalidateRect(hdlg, NULL, TRUE);
		::UpdateWindow(hdlg);

		return FALSE;
	}

	BOOL win32_error_dialog::on_wm_getminmaxinfo(HWND hdlg, WPARAM wparam, LPARAM lparam) {
		UNUSED_PARAMETER(hdlg);
		UNUSED_PARAMETER(wparam);
		reinterpret_cast<MINMAXINFO*>(lparam)->ptMinTrackSize.x = _min_track_size.get_width();
		reinterpret_cast<MINMAXINFO*>(lparam)->ptMinTrackSize.y = _min_track_size.get_height();
		return TRUE;
	}

}