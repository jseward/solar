#pragma once

#include "win32_headers.h"
#include "win32_dialog_control_anchor.h"
#include "solar/math/rect.h"
#include "solar/math/size.h"

namespace solar {

	class win32_dialog_control {
	private:
		const win32_dialog_control_anchor _anchor;
		const int _resource_id;

		rect _initial_area_in_dlg_space;
		size _initial_dlg_size;

	public:
		win32_dialog_control(win32_dialog_control_anchor anchor, int resource_id);

		void on_wm_initdialog(HWND hdlg, WPARAM wparam, LPARAM lparam);
		void on_wm_size(HWND hdlg, WPARAM wparam, LPARAM lparam);
	};

}