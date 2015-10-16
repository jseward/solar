#include "dialog_window_style.h"

#include "solar/archiving/archiving_helpers.h"

namespace solar {

	window_style_factory* dialog_window_style_id::_s_factory = nullptr;

	const char* dialog_window_style::TYPE_NAME = "dialog_window";

	dialog_window_style::dialog_window_style() {
	}

	dialog_window_style::~dialog_window_style() {
	}

	std::unique_ptr<window_style> dialog_window_style::clone() const {
		return std::make_unique<dialog_window_style>();
	}

	void dialog_window_style::read_from_archive(archive_reader& reader) {
		read_object(reader, "title_layout", _title_layout);
		read_object(reader, "content_layout", _content_layout);
		read_object(reader, "close_button_layout", _close_button_layout);
		read_object(reader, "frame_underlay", _frame_underlay);
		read_color(reader, "parent_fade_color", _parent_fade_color);
		read_object(reader, "title_font_def", _title_font_def);
	}

	void dialog_window_style::write_to_archive(archive_writer& writer) const {
		write_object(writer, "title_layout", _title_layout);
		write_object(writer, "content_layout", _content_layout);
		write_object(writer, "close_button_layout", _close_button_layout);
		write_object(writer, "frame_underlay", _frame_underlay);
		write_color(writer, "parent_fade_color", _parent_fade_color);
		write_object(writer, "title_font_def", _title_font_def);
	}

}