#pragma once

#include <array>
#include "solar/windows/window_component.h"
#include "solar/windows/styling/window_style_id.h"
#include "solar/rendering/brushes/brush_id.h"
#include "solar/text/text_id.h"
#include "solar/input/mouse_button.h"
#include "button_style.h"
#include "button_press_params.h"

namespace solar {

	class button : public window_component {
	public:
		using is_toggled_callback_function = std::function<bool()>;
		using pressed_callback_function = std::function<void(const button_press_params&)>;
		using get_text_callback_function = std::function<const wchar_t*()>;

	private:
		button_style_id _style;
		brush_id _icon;
		text_id _text;
		is_toggled_callback_function _is_toggled_callback;
		pressed_callback_function _pressed_callback;
		get_text_callback_function _get_text_callback;
		enum_array<bool, mouse_button> _is_mouse_button_down;

	public:
		button(const char* id);
		virtual ~button();

		button& set_is_toggled_callback(is_toggled_callback_function callback);
		button& set_pressed_callback(pressed_callback_function callback);
		button& set_get_text_callback(get_text_callback_function get_text_callback);

		const wchar_t* get_text() const;

	protected:
		virtual void render(const window_render_params& params) override;
		virtual bool on_mouse_button_down(const window_mouse_button_event_params& params) override;
		virtual bool on_mouse_button_up(const window_mouse_button_event_params& params) override;
		virtual bool on_mouse_button_up_anywhere(const window_mouse_button_event_params& params) override;

	protected:
		virtual void read_from_archive(archive_reader& reader) override;
		virtual void write_to_archive(archive_writer& writer) const override;
	};

}