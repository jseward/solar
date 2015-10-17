#pragma once

#include "solar/containers/enum_array.h"
#include "solar/windows/rendering/window_render_state.h"
#include "solar/windows/window_font_def.h"
#include "solar/windows/layout/window_layout.h"
#include "solar/windows/styling/window_style.h"
#include "solar/windows/styling/window_style_id.h"
#include "solar/windows/rendering/window_render_state_brush_collection.h"

namespace solar {

	class button_style : public window_style {
	public:
		static const char* TYPE_NAME;

	public:
		window_layout _icon_layout;
		window_render_state_brush_collection _underlay_brushes;
		window_render_state_brush_collection _toggled_underlay_brushes;
		window_font_def _font_def;

	public:
		button_style();

	public:
		virtual std::unique_ptr<window_style> clone() const override;
		virtual void read_from_archive(archive_reader& reader) override;
		virtual void write_to_archive(archive_writer& writer) const override;
	};

	typedef window_style_id<button_style> button_style_id;

}
