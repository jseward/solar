#pragma once

#include "solar/windows/styling/window_style.h"
#include "solar/windows/styling/window_style_id.h"
#include "solar/windows/layout/window_layout_margins.h"
#include "solar/windows/window_font_def.h"
#include "solar/rendering/brushes/brush_id.h"

namespace solar {

	class text_entry_style : public window_style {
	public:
		static const char* TYPE_NAME;

	public:
		brush_id _underlay;
		brush_id _selection_underlay;
		window_layout_margins _text_margins;
		window_font_def _font;
		int _caret_width;
		float _caret_blink_time_period;
		color _caret_color_begin;
		color _caret_color_end;

	public:
		text_entry_style();

	public:
		virtual std::unique_ptr<window_style> clone() const override;
		virtual void read_from_archive(archive_reader& reader) override;
		virtual void write_to_archive(archive_writer& writer) const override;
	};

	typedef window_style_id<text_entry_style> text_entry_style_id;

}
