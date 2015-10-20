#include "text_entry_style.h"

#include "solar/archiving/archiving_helpers.h"

namespace solar {

	window_style_factory* text_entry_style_id::_s_factory = nullptr;

	const char* text_entry_style::TYPE_NAME = "text_entry";

	text_entry_style::text_entry_style() 
		: _caret_width(0)
		, _caret_blink_time_period(0.f) {
	}

	std::unique_ptr<window_style> text_entry_style::clone() const {
		return std::make_unique<text_entry_style>();
	}

	void text_entry_style::read_from_archive(archive_reader& reader) {
		read_object(reader, "underlay", _underlay);
		read_object(reader, "selection_underlay", _selection_underlay);
		read_object(reader, "text_margins", _text_margins);
		read_object(reader, "font", _font);
		read_int(reader, "caret_width", _caret_width);
		read_float(reader, "caret_blink_time_period", _caret_blink_time_period);
		read_color(reader, "caret_color_begin", _caret_color_begin);
		read_color(reader, "caret_color_end", _caret_color_end);
	}

	void text_entry_style::write_to_archive(archive_writer& writer) const {
		write_object(writer, "underlay", _underlay);
		write_object(writer, "selection_underlay", _selection_underlay);
		write_object(writer, "text_margins", _text_margins);
		write_object(writer, "font", _font);
		write_int(writer, "caret_width", _caret_width);
		write_float(writer, "caret_blink_time_period", _caret_blink_time_period);
		write_color(writer, "caret_color_begin", _caret_color_begin);
		write_color(writer, "caret_color_end", _caret_color_end);
	}

}