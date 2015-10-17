#include "dialog_content_window.h"

#include "dialog_window.h"

namespace solar {

	dialog_content_window::dialog_content_window(dialog_window& dialog_window)
		: window("dialog_content_window") 
		, _dialog_window(dialog_window) {
	}

	void dialog_content_window::on_parent_area_changed() {
		set_area(_dialog_window.get_style()._content_layout.build_area(get_parent()));
	}

}