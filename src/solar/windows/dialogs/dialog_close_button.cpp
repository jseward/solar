#include "dialog_close_button.h"

#include "dialog_window.h"
#include "solar/windows/rendering/window_renderer.h"
#include "solar/windows/rendering/window_render_state_helpers.h"

namespace solar {

	dialog_close_button::dialog_close_button(dialog_window& dialog_window)
		: window("dialog_close_button") 
		, _dialog_window(dialog_window) {

		set_is_visible_callback([this]() { return _dialog_window.is_closable_ever(); });
	}

	void dialog_close_button::on_parent_area_changed() {
		set_area(_dialog_window.get_style()._close_button_layout.build_area(get_parent()));
	}

	bool dialog_close_button::on_mouse_button_up(const window_mouse_button_event_params& params) {
		_dialog_window.try_close();
		return true;
	}

	void dialog_close_button::render(const window_render_params& params) {
		auto state = make_best_window_render_state(*this, params);
		params._window_renderer.begin_brush_rendering();
		params._window_renderer.render_brush(*this, _dialog_window.get_style()._close_button_brushes.get(state));
		params._window_renderer.end_brush_rendering();
	}

}