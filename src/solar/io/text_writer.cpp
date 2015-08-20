#include "text_writer.h"

#include "solar/strings/string_helpers.h"
#include "solar/utility/verify.h"

namespace solar {

	text_writer::text_writer(stream* stream)
		: _stream(stream) {
	}

	text_writer::~text_writer() {
	}

	void text_writer::write(const std::string& text) {
		IF_VERIFY(_stream != nullptr) {
			_stream->write_bytes(text.c_str(), text.length());
		}
	}

	void text_writer::write(const std::string& format, fmt::ArgList args) {
		write(fmt::format(format, args));
	}

	void text_writer::write_line(const std::string& text) {
		IF_VERIFY(_stream) {
			_stream->write_bytes(text.c_str(), text.length());
			const char* new_line = "\n";
			_stream->write_bytes(new_line, 1);
		}
	}

	void text_writer::write_line(const std::string& format, fmt::ArgList args) {
		write_line(fmt::format(format, args));
	}

}
