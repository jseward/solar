#pragma once

#include "solar/windows/styling/window_style.h"
#include "solar/windows/styling/window_style_id.h"
#include "solar/windows/layout/window_layout.h"
#include "solar/windows/window_font_def.h"
#include "solar/windows/rendering/window_render_state_brush_collection.h"
#include "solar/rendering/brushes/brush_id.h"
#include "solar/colors/color.h"

namespace solar {

	class dialog_window_style : public window_style {
	public:
		static const char* TYPE_NAME;

	public:
		window_layout _title_layout;
		window_layout _content_layout;
		window_layout _close_button_layout;
		brush_id _frame_underlay;
		color _parent_fade_color;
		window_font_def _title_font_def;
		window_render_state_brush_collection _close_button_brushes;

	public:
		dialog_window_style();
		virtual ~dialog_window_style();

	public:
		virtual std::unique_ptr<window_style> clone() const override;
		virtual void read_from_archive(archive_reader& reader) override;
		virtual void write_to_archive(archive_writer& writer) const override;
	};

	typedef window_style_id<dialog_window_style> dialog_window_style_id;

}
