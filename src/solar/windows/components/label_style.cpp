#include "label_style.h"

#include "solar/archiving/archiving_helpers.h"

namespace solar {

	window_style_factory* label_style_id::_s_factory = nullptr;

	const char* label_style::TYPE_NAME = "label";

	std::unique_ptr<window_style> label_style::clone() const {
		return std::make_unique<label_style>();
	}

	void label_style::read_from_archive(archive_reader& reader) {
		read_object(reader, "font_def", _font_def);
	}

	void label_style::write_to_archive(archive_writer& writer) const {
		write_object(writer, "font_def", _font_def);
	}

}
