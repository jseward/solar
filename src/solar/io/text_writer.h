#pragma once

#include <string>
#include "solar_dependencies/cppformat/format.h"
#include "stream.h"

namespace solar {

	class text_writer {
	private:
		stream* _stream;

	public:
		text_writer(stream* stream);
		virtual ~text_writer();

		void write(const std::string& text);
		void write(const std::string& format, fmt::ArgList args);
		FMT_VARIADIC(void, write, const char*)

		void write_line(const std::string& text);
		void write_line(const std::string& format, fmt::ArgList args);
		FMT_VARIADIC(void, write_line, const char*)
	};

}