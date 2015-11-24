#include "dialog_title_window.h"

#include "dialog_window.h"
#include "solar/windows/rendering/window_renderer.h"

namespace solar {

	dialog_title_window::dialog_title_window(dialog_window& dialog_window)
		: window("dialog_title_window") 
		, _dialog_window(dialog_window) {
	}

	void dialog_title_window::on_parent_area_changed() {
		set_area(_dialog_window.get_style()._title_layout.build_area(get_parent()));
	}

	void dialog_title_window::render(const window_render_params& params) {
		auto& renderer = params._window_renderer;

		renderer.begin_font_rendering();
		renderer.render_font(*this, _dialog_window.get_style()._title_font_def, _dialog_window.get_title_text());
		renderer.end_font_rendering();
	}

}