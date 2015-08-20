#pragma once

#include <memory>
#include <string>
#include "win32_headers.h"
#include "win32_error_dialog_resources.h"
#include "win32_dialog_control.h"
#include "solar/math/size.h"

namespace solar{

	enum class win32_error_dialog_result {
		BREAK,
		SKIP,
		SKIP_ALL
	};

	class win32_error_dialog {

	private:
		win32_error_dialog_resources _resources;
		HWND _hwnd;
		HINSTANCE _hinstance;
		DWORD _main_thread_id;
		RECT _saved_dlg_rect;
		std::unique_ptr<win32_dialog_control> _textbox;
		std::unique_ptr<win32_dialog_control> _break_button;
		std::unique_ptr<win32_dialog_control> _skip_button;
		std::unique_ptr<win32_dialog_control> _skip_all_button_button;

		bool _is_first_init_dialog;
		size _min_track_size;
		std::string _title;
		std::string _message;

	public:
		win32_error_dialog();

		void setup(const win32_error_dialog_resources& resources, HWND hwnd, HINSTANCE hinstance);

		win32_error_dialog_result show(const char* title, const char* message);

	private:
		static win32_error_dialog* s_dlg_proc_context;
		static INT_PTR CALLBACK dlg_proc(HWND hdlg, UINT msg, WPARAM wparams, LPARAM lparam);

	private:
		BOOL on_wm_initdialog(HWND hdlg, WPARAM wparam, LPARAM lparam);
		BOOL on_wm_command(HWND hdlg, WPARAM wparam, LPARAM lparam);
		BOOL on_wm_size(HWND hdlg, WPARAM wparam, LPARAM lparam);
		BOOL on_wm_getminmaxinfo(HWND hdlg, WPARAM wparam, LPARAM lparam);
	};

}