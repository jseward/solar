#include "font_renderer_line_builder.h"
#include "solar/utility/assert.h"
#include "solar/utility/optional.h"
#include "solar/utility/type_convert.h"

namespace solar {

	font_renderer_line_builder::font_renderer_line_builder() {
		_lines.reserve(10);
	}

	void font_renderer_line_builder::build_lines(const font_render_params& params) {
		_lines.clear();
		
		if (params._is_multiline) {
			build_lines_multiline(params);
		}
		else {
			build_lines_singleline(params);
		}
	}

	const std::vector<font_renderer_line>& font_renderer_line_builder::get_lines() const {
		return _lines;
	}

	void font_renderer_line_builder::build_lines_singleline(const font_render_params& params) {
		ASSERT(_lines.empty());

		float x = int_to_float(params._render_area.get_left());
		float y = int_to_float(params._render_area.get_top());

		if (font_alignment_is_center_horizontal(params._alignment)) {
			float width = params._font.get_text_width(params._font_size, params._text);
			x = int_to_float(params._render_area.get_center_x()) - (width / 2.f);
		}
		else if (font_alignment_is_right(params._alignment)) {
			float width = params._font.get_text_width(params._font_size, params._text);
			x = int_to_float(params._render_area.get_right()) - width;
		}

		if (font_alignment_is_center_vertical(params._alignment)) {
			float height = params._font.get_line_height(params._font_size);
			y = int_to_float(params._render_area.get_center_y()) - (height / 2.f);
		}
		else if (font_alignment_is_bottom(params._alignment)) {
			float height = params._font.get_line_height(params._font_size);
			y = int_to_float(params._render_area.get_bottom()) - height;
		}

		if (!params._is_clipping_enabled) {
			_lines.emplace_back(0, get_string_length(params._text), pointf(x, y));
		}
		else {

			//either extents of the text could be outside the render area (consider text centered horizontally)
			unsigned int len = get_string_length(params._text);

			optional<unsigned int> begin;
			unsigned int end = len;

			float curr_x = x;
			for (unsigned int i = 0; i < len; ++i) {
				if (curr_x > params._render_area.get_right()) {
					end = i;
					break;
				}
				curr_x += params._font.get_char_width(params._font_size, params._text[i]);
				if (curr_x > params._render_area.get_left()) {
					if (!begin.has_value()) {
						begin = i;
					}
				}
			}

			_lines.emplace_back(begin.get_value_or(0), len, pointf(x, y));
		}
	}

	void font_renderer_line_builder::build_lines_multiline(const font_render_params& params) {
		ASSERT(_lines.empty());
		ASSERT(params._alignment == font_alignment::TOP_LEFT);

		const wchar_t* text = params._text;
		const unsigned int text_len = get_string_length(params._text);
		const float line_height = params._font.get_line_height(params._font_size);
		const float render_area_width = int_to_float(params._render_area.get_width());

		float curr_width = 0.f;
		float curr_x = int_to_float(params._render_area.get_left());
		float curr_y = int_to_float(params._render_area.get_top());
		bool has_word_break_index = false;
		unsigned int word_break_index = 0;
		unsigned int begin_index = 0;
		bool is_begin_of_new_line = true;
		bool is_clipped = false;

		unsigned int curr_index = 0;
		while (curr_index < text_len && !is_clipped) {

			if (is_word_break_char(text[curr_index])) {
				has_word_break_index = true;
				word_break_index = curr_index;
			}

			bool should_advance_to_next_line = (text[curr_index] == L'\n');
			if (!should_advance_to_next_line) {
				curr_width += params._font.get_char_width(params._font_size, text[curr_index]);

				if (is_begin_of_new_line) {
					is_begin_of_new_line = false;
				}
				else {
					curr_width += params._font.get_kerning_pair_offset(params._font_size, text[curr_index - 1], text[curr_index]);
					if (curr_width > render_area_width) {
						should_advance_to_next_line = true;
					}
				}
			}

			if (should_advance_to_next_line) {
				unsigned int end_index;
				if (has_word_break_index) {
					end_index = word_break_index;
					curr_index = end_index + 1;
				}
				else {
					//We hit the right edge but we don't have a word break character to break on (e.g happens a lot with Asian text). 
					//Use the current index as a fallback.
					end_index = curr_index;
				}

				_lines.emplace_back(begin_index, end_index, pointf(curr_x, curr_y));

				curr_width = 0.f;
				has_word_break_index = false;
				word_break_index = 0;
				begin_index = curr_index;
				is_begin_of_new_line = true;
				curr_y += line_height;

				if (params._is_clipping_enabled && curr_y > params._render_area.get_bottom()) {
					is_clipped = true;
				}
			}
			else {
				curr_index++;
			}
		}

		if (begin_index >= 0 && begin_index < text_len) {
			if (!is_clipped) {
				_lines.emplace_back(begin_index, text_len, pointf(curr_x, curr_y));
			}
		}
	}

	bool font_renderer_line_builder::is_word_break_char(wchar_t c) const {
		return
			(c == L' ') ||
			(c == L'\n');
	}
}