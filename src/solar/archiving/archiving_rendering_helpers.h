#pragma once

#include "solar/archiving/archive_reader.h"
#include "solar/archiving/archive_writer.h"
#include "solar/rendering/color.h"

namespace solar {

	inline void read_color(archive_reader& reader, const char* name, color& c) {
		std::string s;
		reader.read_string(name, s);
		if (!try_make_color_from_string(c, s.c_str())) {
			reader.raise_error(build_string("failed to parse color from string : \"{}\"", s));
		}
	}

	inline void write_color(archive_writer& writer, const char* name, const color& c) {
		writer.write_string(name, c.to_string());
	}

}