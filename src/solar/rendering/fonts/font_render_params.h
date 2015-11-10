#pragma once

#include "solar/math/rect.h"
#include "solar/colors/color.h"
#include "solar/colors/colors.h"
#include "font.h"
#include "font_alignment.h"
#include "font_dropshadow_def.h"

namespace solar {

	class font_render_params {
	public:
		font& _font;
		float _font_size;
		const wchar_t* _text;
		font_alignment _alignment;
		bool _is_multiline;
		bool _is_clipping_enabled;
		rect _render_area;
		color _color;
		bool _is_dropshadow_enabled;
		font_dropshadow_def _dropshadow_def;

	public:
		font_render_params(font& font, float font_size, const wchar_t* text)
			: _font(font)
			, _font_size(font_size)
			, _text(text)
			, _alignment(font_alignment::TOP_LEFT)
			, _is_multiline(false)
			, _is_clipping_enabled(false)
			, _render_area()
			, _color(colors::WHITE)
			, _is_dropshadow_enabled(false)
			, _dropshadow_def() {
		}

		font_render_params& set_alignment(font_alignment alignment) {
			_alignment = alignment;
			return *this;
		}

		font_render_params& set_is_multiline(bool is_multiline) {
			_is_multiline = is_multiline;
			return *this;
		}

		font_render_params& set_is_clipping_enabled(bool is_clipping_enabled) {
			_is_clipping_enabled = is_clipping_enabled;
			return *this;
		}

		font_render_params& set_render_area(const rect& render_area) {
			_render_area = render_area;
			return *this;
		}

		font_render_params& set_color(const color& color) {
			_color = color;
			return *this;
		}

		font_render_params& set_is_dropshadow_enabled(bool is_dropshadow_enabled) {
			_is_dropshadow_enabled = is_dropshadow_enabled;
			return *this;
		}

		font_render_params& set_dropshadow_def(const font_dropshadow_def& dropshadow_def) {
			_dropshadow_def = dropshadow_def;
			return *this;
		}
	};

}