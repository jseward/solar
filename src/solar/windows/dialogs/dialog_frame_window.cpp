#include "dialog_frame_window.h"

#include "solar/windows/window_renderer.h"
#include "dialog_window.h"

namespace solar {

	dialog_frame_window::dialog_frame_window(dialog_window& dialog_window)
		: window("dialog_frame_window")
		, _dialog_window(dialog_window) {
	}

	void dialog_frame_window::on_parent_area_changed() {
		set_area(_dialog_window.get_frame_layout().build_area(get_parent()));
	}

	void dialog_frame_window::render(const window_render_params& params) {
		params._window_renderer.begin_brush_rendering(window_render_state::NORMAL);
		params._window_renderer.render_brush(*this, _dialog_window.get_style()._frame_underlay, brush_render_mode::STRETCHED);
		params._window_renderer.end_brush_rendering();
	}

}