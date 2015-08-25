#pragma once

#include "solar/math/rect.h"
#include "solar/math/point.h"
#include "solar/math/pointf.h"
#include "solar/math/vec3.h"
#include "archiving_helpers.h"

namespace solar {

	inline void read_rect(archive_reader& reader, const char* name, rect& value) {
		int left = 0;
		int top = 0;
		int right = 0;
		int bottom = 0;
		read_ints(reader, name, left, top, right, bottom);
		value = rect(point(left, top), point(right, bottom));
	}

	inline void write_rect(archive_writer& writer, const char* name, const rect& value) {
		write_ints(writer, name, value.get_left(), value.get_top(), value.get_right(), value.get_bottom());
	}

	inline void read_point(archive_reader& reader, const char* name, point& value) {
		int x = 0;
		int y = 0;
		read_ints(reader, name, x, y);
		value = point(x, y);
	}

	inline void write_point(archive_writer& writer, const char* name, const point& value) {
		write_ints(writer, name, value._x, value._y);
	}

	inline void read_size(archive_reader& reader, const char* name, size& value) {
		int width = 0;
		int height = 0;
		read_ints(reader, name, width, height);
		value = size(width, height);
	}

	inline void write_size(archive_writer& writer, const char* name, const size& value) {
		write_ints(writer, name, value._width, value._height);
	}

	inline void read_pointf(archive_reader& reader, const char* name, pointf& value) {
		float x = 0;
		float y = 0;
		read_floats(reader, name, x, y);
		value = pointf(x, y);
	}

	inline void write_pointf(archive_writer& writer, const char* name, const pointf& value) {
		write_floats(writer, name, value._x, value._y);
	}

	inline void read_vec3(archive_reader& reader, const char* name, vec3& value) {
		float x = 0;
		float y = 0;
		float z = 0;
		read_floats(reader, name, x, y, z);
		value = vec3(x, y, z);
	}

	inline void write_vec3(archive_writer& writer, const char* name, const vec3& value) {
		write_floats(writer, name, value._x, value._y, value._z);
	}

}
