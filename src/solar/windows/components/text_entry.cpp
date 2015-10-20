#include "text_entry.h"

#include "solar/archiving/archiving_helpers.h"
#include "solar/windows/rendering/window_render_state_helpers.h"
#include "solar/windows/rendering/window_renderer.h"
#include "solar/utility/unused_parameter.h"
#include "solar/utility/trace.h"
#include "solar/rendering/primatives/prim2d.h"
#include "solar/rendering/primatives/prim2d_lines.h"
#include "solar/math/math_type_convert.h"
#include "solar/math/math_helpers.h"
#include "solar/time/real_time.h"

namespace solar {

	text_entry::text_entry(const char* id) 
		: window_component(id)
		, _style(*this) 

		, _max_text_length(128)
		, _is_password(false)
		, _is_read_only(false)

		, _visible_pos(0)
		, _caret_pos(0) 
		, _selection_pos(0)
		, _is_dragging_selection(false) {
	}

	text_entry::~text_entry() {
	}

	void text_entry::set_text_ignore_change(const wchar_t* new_text) {
		copy_text_to_fit(new_text);
		_caret_pos = _text.length();
		_selection_pos = _caret_pos;
		make_caret_visible();
	}

	void text_entry::copy_text_to_fit(const wchar_t* new_text) {
		auto new_length = get_string_length(new_text);
		if (new_length > _max_text_length) {
			TRACE("WARNING : text is too large for text_entry:{}", get_id());
		}
		_text.assign(new_text, std::min(new_length, _max_text_length));
	}

	void text_entry::make_caret_visible() {
		if (_caret_pos < _visible_pos) {
			_visible_pos = _caret_pos;
		}
		else {
			int width_available = get_text_area().get_width();
			while (_visible_pos < (uint_to_int(_text.length()) - 1)) {
				if (get_caret_offset_from_left() <= width_available) {
					break;
				}
				++_visible_pos;
			}
		}
	}

	bool text_entry::is_focusable_ever() const {
		return true;
	}

	void text_entry::render(const window_render_params& params) {
		refresh_render_text_parts();

		auto& renderer = params._window_renderer;

		renderer.begin_brush_rendering();
		renderer.render_brush(*this, _style->_underlay);
		if (_render_text_parts._selection._area.get_width() > 0.f) {
			renderer.render_brush(*this, _style->_selection_underlay, brush_render_mode::STRETCHED, rectf_to_rect(_render_text_parts._selection._area));
		}		
		renderer.end_brush_rendering();

		renderer.begin_font_rendering();
		renderer.render_font(*this, _style->_font, _render_text_parts._pre_selection._clipped_text.c_str(), rectf_to_rect(_render_text_parts._pre_selection._area));
		renderer.render_font(*this, _style->_font, _render_text_parts._selection._clipped_text.c_str(), rectf_to_rect(_render_text_parts._selection._area));
		renderer.render_font(*this, _style->_font, _render_text_parts._post_selection._clipped_text.c_str(), rectf_to_rect(_render_text_parts._post_selection._area));
		renderer.end_font_rendering();

		renderer.get_prim2d().begin_rendering(get_area());
		renderer.get_prim2d().render_rect(get_caret_area(), get_caret_color());
		renderer.get_prim2d().end_rendering();
	}

	bool text_entry::on_mouse_button_down(const window_mouse_button_event_params& params) {
		try_make_focused();

		_caret_pos = get_cursor_caret_pos(params._cursor_pos);
		if (!params.is_shift_down()) {
			_selection_pos = _caret_pos;
			_is_dragging_selection = true;
		}

		return true;
	}

	bool text_entry::on_mouse_button_up_anywhere(const window_mouse_button_event_params& params) {
		UNUSED_PARAMETER(params);
		_is_dragging_selection = false;
		return false;
	}

	int text_entry::get_cursor_caret_pos(const point& cursor_pos) const {
		int cursor_caret_pos = _visible_pos;

		int text_area_left = get_text_area().get_left();
		if (cursor_pos._x >= text_area_left) {
			
			auto& font = _style->_font._font_id.get();
			float font_size = _style->_font.get_scaled_font_size(get_font_scale());

			std::wstring visible_text = _text;
			raw_text_to_visible_text(visible_text);
			
			int text_length = uint_to_int(visible_text.length());
			while (cursor_caret_pos < text_length) {
				float width_a = font.get_text_width(font_size, visible_text.c_str(), _visible_pos, cursor_caret_pos - _visible_pos);
				float width_b = font.get_text_width(font_size, visible_text.c_str(), _visible_pos, cursor_caret_pos - _visible_pos + 1);
				float width_extra = (width_b - width_a) / 2.f;
				if (cursor_pos._x < (int_to_float(text_area_left) + width_a + width_extra)) {
					break;
				}
				cursor_caret_pos++;
			}
		}

		return cursor_caret_pos;
	}

	int text_entry::get_caret_offset_from_left() const {
		ASSERT(_caret_pos <= uint_to_int(_text.length()));
		int caret_pos_in_visible_text = _caret_pos - _visible_pos;
		if (caret_pos_in_visible_text >= 0) {
			std::wstring visible_text = std::wstring(_text, _visible_pos, caret_pos_in_visible_text);
			raw_text_to_visible_text(visible_text);
			return float_to_int(std::ceil(get_text_width(visible_text)));
		}
		return 0;
	}

	rect text_entry::get_caret_area() const {
		rect text_area = get_text_area();
		int caret_left = text_area.get_left() + get_caret_offset_from_left();
		return rect(point(caret_left, text_area.get_top()), size(_style->_caret_width, text_area.get_height()));
	}

	color text_entry::get_caret_color() const {
		if (is_focused()) {
			if (_is_read_only) {
				//no blinking when read only, looks like user can type if blinking.
				return _style->_caret_color_begin;
			}
			else {
				float t = calculate_oscillate_t(get_real_time_in_seconds(), _style->_caret_blink_time_period);
				return lerp(_style->_caret_color_begin, _style->_caret_color_end, t);
			}
		}
		return color(0.f, 0.f, 0.f, 0.f);
	}

	void text_entry::read_from_archive(archive_reader& reader) {
		window_component::read_from_archive(reader);
		read_object(reader, "style", _style);
	}
	
	void text_entry::write_to_archive(archive_writer& writer) const {
		window_component::write_to_archive(writer);
		write_object(writer, "style", _style);
	}

	void text_entry::refresh_render_text_parts() {
		rectf text_area = rect_to_rectf(get_text_area());

		refresh_render_text_part(
			_render_text_parts._pre_selection,
			nullptr,
			0,
			get_selection_begin(),
			text_area);

		refresh_render_text_part(
			_render_text_parts._selection,
			&_render_text_parts._pre_selection,
			get_selection_begin(),
			get_selection_end(),
			text_area);

		refresh_render_text_part(
			_render_text_parts._post_selection,
			&_render_text_parts._selection,
			get_selection_end(),
			_text.length(),
			text_area);
	}

	void text_entry::refresh_render_text_part(render_text_part& part, const render_text_part* previous_part, int begin_pos, int end_pos, const rectf& all_parts_area) {
		int visible_begin_pos = std::max(_visible_pos, begin_pos);
		int unclipped_char_count = std::max(0, end_pos - visible_begin_pos);
		part._unclipped_text.assign(_text.c_str(), visible_begin_pos, unclipped_char_count);
		raw_text_to_visible_text(part._unclipped_text);
		
		part._clipped_text.assign(L"");
		float clipped_text_width = 0.f;

		pointf clipped_top_left = all_parts_area.get_top_left();
		if (previous_part != nullptr) {
			clipped_top_left = previous_part->_area.get_top_right();
		}
		const float available_width = all_parts_area.get_right() - clipped_top_left._x;

		size_t clipped_char_count = part._unclipped_text.length();
		while (clipped_char_count >= 0) {
			part._clipped_text.assign(part._unclipped_text.c_str(), 0, clipped_char_count);
			
			clipped_text_width = get_text_width(part._clipped_text);
			if (previous_part != nullptr && !previous_part->_clipped_text.empty()) {
				clipped_text_width += get_kerning_width(
					previous_part->_clipped_text[previous_part->_clipped_text.length() - 1],
					part._clipped_text[0]);
			}
			
			if (clipped_text_width <= available_width) {
				break;
			}
		}

		part._area = rectf(clipped_top_left, sizef(clipped_text_width, all_parts_area.get_height()));
	}

	void text_entry::raw_text_to_visible_text(std::wstring& s) const {
		if (_is_password) {
			size_t length = s.length();
			for (unsigned int i = 0; i < length; ++i) {
				s[i] = L'*';
			}
		}
		else {
			//do nothing
		}
	}

	rect text_entry::get_text_area() const {
		return _style->_text_margins.transform_area(get_area(), get_area_scale());
	}

	float text_entry::get_text_width(const std::wstring& text) const {
		return _style->_font._font_id->get_text_width(
			_style->_font.get_scaled_font_size(get_font_scale()), 
			text.c_str());
	}

	float text_entry::get_kerning_width(wchar_t a, wchar_t b) const {
		//todo - kerning
		UNUSED_PARAMETER(a);
		UNUSED_PARAMETER(b);
		return 0.f;
	}

	int text_entry::get_selection_begin() const {
		return std::min(_caret_pos, _selection_pos);
	}

	int text_entry::get_selection_end() const {
		return get_selection_begin() + get_selection_length();
	}

	int text_entry::get_selection_length() const {
		return std::abs(_caret_pos - _selection_pos);
	}

}