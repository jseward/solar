#pragma once

#include "solar/utility/property.h"

namespace solar{

	struct win32_error_dialog_resources {
		MAKE_PROPERTY(win32_error_dialog_resources, unsigned int, dialog_id, 0);
		MAKE_PROPERTY(win32_error_dialog_resources, unsigned int, textbox_id, 0);
		MAKE_PROPERTY(win32_error_dialog_resources, unsigned int, break_button_id, 0);
		MAKE_PROPERTY(win32_error_dialog_resources, unsigned int, skip_button_id, 0);
		MAKE_PROPERTY(win32_error_dialog_resources, unsigned int, skip_all_button_id, 0);
	};

}