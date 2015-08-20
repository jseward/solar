#include "button.h"
#include "solar/windows/window_render_params.h"
#include "solar/windows/window_renderer.h"
#include "solar/windows/window_render_state_helpers.h"
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

	button& button::set_pressed_callback(std::function<void(button_press_params)> callback) {
		ASSERT(_pressed_callback == nullptr);
		_pressed_callback = callback;
		return *this;
	}

	const wchar_t* button::get_text() const {
		return _text.get();
	}

	void button::render(const window_render_params& params) {
		auto& renderer = params.get_window_renderer();
		
		auto state = get_best_window_render_state(*this, params);
		renderer.begin_brush_rendering(state);
		renderer.render_brush(*this, _style.get()._underlay, brush_render_mode::STRETCHED);
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
		if (is_enabled()) {
			if (_is_mouse_button_down.at(params._mouse_button)) {
				_is_mouse_button_down.at(params._mouse_button) = false;
				_pressed_callback(button_press_params(*this).set_mouse_button(params._mouse_button));
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
		read_object(reader, "text", _text);
	}

	void button::write_to_archive(archive_writer& writer) const {
		window_component::write_to_archive(writer);
		write_object(writer, "style", _style);
		write_object(writer, "icon", _icon);
		write_object(writer, "text", _text);
	}

}