#include "text_reader.h"

#include "solar/utility/verify.h"
#include "solar/utility/alert.h"

namespace solar {

	text_reader::text_reader(stream* stream)
		: _stream(stream) 
		, _line_number(0)
		, _read_buffer_size(0)
		, _read_buffer_pos(0)
		, _has_read_past_BOM(false) {
	}

	text_reader::~text_reader() {
	}

	bool text_reader::read_line(std::string& line) {
		line.clear();

		int read_char_count = 0;
		IF_VERIFY(_stream != nullptr) {
			attempt_read_past_BOM();

			bool has_slash_r = false;

			for (;;) {
				if (_read_buffer_pos >= _read_buffer_size) {
					_read_buffer_pos = 0;
					_read_buffer_size = _stream->read_bytes(_read_buffer.data(), _read_buffer.size());
					if (_read_buffer_size == 0) {
						break;
					}
				}

				if (_read_buffer_size > 0) {
					ASSERT(_read_buffer_pos < _read_buffer_size);
					char read_char = _read_buffer.at(_read_buffer_pos);
					_read_buffer_pos++;
					read_char_count++;

					if (!has_slash_r) {
						if (read_char == '\r') {
							has_slash_r = true;
						}
						else if (read_char == '\n') {
							break;
						}
						else {
							line.append(1, read_char);
						}
					}
					else {
						if (read_char == '\n') {
							break;
						}
						else {
							alert_read_error(R"(Expected \n after \r.)");
							break;
						}
					}
				}
			}
		}

		if (read_char_count > 0) {
			_line_number += 1;
		}

		return (read_char_count > 0);
	}

	int text_reader::get_line_number() const {
		return _line_number;
	}

	void text_reader::alert_read_error(const char* reason) {
		ALERT("text_reader error.\nreason : {}\nstream : {}\nline_number : {}",
			reason,
			_stream != nullptr ? _stream->get_description() : "null",
			_line_number);
	}

	void text_reader::attempt_read_past_BOM() {
		if (!_has_read_past_BOM) {
			_has_read_past_BOM = true;

			if (_stream != nullptr && _stream->can_have_BOM()) {
				bool was_BOM_read = false;

				std::array<char, 3> read_bytes;
				if (_stream->read_bytes(read_bytes.data(), read_bytes.size()) == read_bytes.size()) {
					std::array<char, 3> BOM_bytes = { '\xEF', '\xBB', '\xBF' };
					if (read_bytes == BOM_bytes) {
						was_BOM_read = true;
					}
				}

				if (!was_BOM_read) {
					_stream->seek(stream_seek_anchor::CURRENT, -3);
				}
			}
		}
	}

}
