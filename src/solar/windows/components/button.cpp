#include "button.h"
#include "solar/windows/rendering/window_render_params.h"
#include "solar/windows/rendering/window_renderer.h"
#include "solar/windows/rendering/window_render_state_helpers.h"
#include "solar/rendering/brushes/brush_renderer.h"
#include "solar/containers/container_helpers.h"

namespace solar {

	button::button(const char* id) 
		: window_component(id)
		, _style(*this) {
		
		set_has_layout(true);
		_is_mouse_button_down.fill(false);
	}

	button::~button() {
	}

	button& button::set_pressed_callback(pressed_callback_function callback) {
		_pressed_callback = callback;
		return *this;
	}

	button& button::set_get_text_callback(get_text_callback_function callback) {
		_get_text_callback = callback;
		return *this;
	}

	button& button::set_get_underlay_callback(get_underlay_callback_function callback) {
		_get_underlay_callback = callback;
		return *this;
	}

	const button_style& button::get_style() const {
		return _style.get();
	}

	const wchar_t* button::get_text() const {
		if (_get_text_callback != nullptr) {
			return _get_text_callback();
		}
		return _text.get();
	}

	const brush* button::get_underlay(window_render_state state) const {
		if (_get_underlay_callback != nullptr) {
			return _get_underlay_callback(state);
		}
		return &_style.get()._underlay_brushes.get(state).get();
	}

	void button::render(const window_render_params& params) {
		auto& renderer = params._window_renderer;
		
		auto state = make_best_window_render_state(*this, params);

		renderer.begin_brush_rendering();		
		renderer.render_brush(*this, get_underlay(state), brush_render_mode::STRETCHED);
		renderer.render_brush(*this, _icon, brush_render_mode::STRETCHED, _style.get()._icon_layout);
		renderer.end_brush_rendering();

		const wchar_t* text = get_text();
		if (!is_string_empty(text)) {
			renderer.begin_font_rendering();
			renderer.render_font(*this, _style.get()._font_def, text);
			renderer.end_font_rendering();
		}
	}

	bool button::on_mouse_button_down(const window_mouse_button_event_params& params) {
		_is_mouse_button_down.at(params._mouse_button) = true;
		return true;
	}

	bool button::on_mouse_button_up(const window_mouse_button_event_params& params) {
		IF_VERIFY(_pressed_callback != nullptr) {
			if (is_enabled()) {
				if (_is_mouse_button_down.at(params._mouse_button)) {
					_is_mouse_button_down.at(params._mouse_button) = false;
					_pressed_callback(button_press_params(*this).set_mouse_button(params._mouse_button));
				}
			}
		}
		return true;
	}

	bool button::on_mouse_button_up_anywhere(const window_mouse_button_event_params& params) {
		_is_mouse_button_down.at(params._mouse_button) = false;
		return false;
	}

	void button::read_from_archive(archive_reader& reader) {
		window_component::read_from_archive(reader);
		read_object(reader, "style", _style);
		read_object(reader, "icon", _icon);
		if (_get_text_callback == nullptr) {
			read_object(reader, "text", _text);
		}
	}

	void button::write_to_archive(archive_writer& writer) const {
		window_component::write_to_archive(writer);
		write_object(writer, "style", _style);
		write_object(writer, "icon", _icon);
		if (_get_text_callback == nullptr) {
			write_object(writer, "text", _text);
		}
	}

}