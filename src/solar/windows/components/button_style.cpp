#include "button_style.h"
#include "solar/archiving/archiving_enum_helpers.h"

namespace solar {

	window_style_factory* button_style_id::_s_factory = nullptr;

	const char* button_style::TYPE_NAME = "button";

	button_style::button_style() {
	}
	
	std::unique_ptr<window_style> button_style::clone() const {
		return std::make_unique<button_style>();
	}

	void button_style::read_from_archive(archive_reader& reader) {
		read_object(reader, "icon_layout", _icon_layout);
		read_object(reader, "underlay_brushes", _underlay_brushes);
		read_object(reader, "font_def", _font_def);
	}

	void button_style::write_to_archive(archive_writer& writer) const {
		write_object(writer, "icon_layout", _icon_layout);
		write_object(writer, "underlay_brushes", _underlay_brushes);
		write_object(writer, "font_def", _font_def);
	}

}