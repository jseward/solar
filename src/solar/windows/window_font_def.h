#pragma once

#include "solar/archiving/archivable.h"
#include "solar/rendering/color.h"
#include "solar/rendering/fonts/font_id.h"
#include "solar/rendering/fonts/font_alignment.h"
#include "solar/rendering/fonts/font_dropshadow_def.h"

namespace solar {

	class window_font_def : public archivable {
	public:
		font_id _font_id;
		float _base_font_size;
		font_alignment _alignment;
		bool _is_multiline;
		bool _is_clipping_enabled;
		color _color;
		bool _is_dropshadow_enabled;
		font_dropshadow_def _dropshadow_def;

	public:
		window_font_def();
		float get_scaled_font_size(float scale) const;

	private:
		void read_from_archive(archive_reader& reader) override;
		void write_to_archive(archive_writer& writer) const override;
	};

}