#include "dialog_content_window.h"

namespace solar {

	dialog_content_window::dialog_content_window(dialog_window& dialog_window)
		: window("dialog_content_window") 
		, _dialog_window(dialog_window) {
	}

}