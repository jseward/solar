#pragma once

#include <array>
#include <memory>
#include "stream.h"

namespace solar {

	class text_reader {
	private:
		stream* _stream;
		int _line_number;
		std::array<char, 1024> _read_buffer;
		unsigned int _read_buffer_size;
		unsigned int _read_buffer_pos;
		bool _has_read_past_BOM;

	public:
		text_reader(stream* stream);
		virtual ~text_reader();

		//Returns true until at end of stream.
		bool read_line(std::string& line);
		int get_line_number() const;

	private:
		void alert_read_error(const char* message);
		void attempt_read_past_BOM();
	};

}