#pragma once

#include "solar/math/rect.h"
#include "solar/math/rectf.h"
#include "solar/math/point.h"
#include "solar/math/pointf.h"
#include "solar/math/vec2.h"
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

	inline void read_rectf(archive_reader& reader, const char* name, rectf& value) {
		float left = 0.f;
		float top = 0.f;
		float right = 0.f;
		float bottom = 0.f;
		read_floats(reader, name, left, top, right, bottom);
		value = rectf(pointf(left, top), pointf(right, bottom));
	}

	inline void write_rectf(archive_writer& writer, const char* name, const rectf& value) {
		write_floats(writer, name, value.get_left(), value.get_top(), value.get_right(), value.get_bottom());
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

	inline void read_vec2(archive_reader& reader, const char* name, vec2& value) {
		float x = 0;
		float y = 0;
		read_floats(reader, name, x, y);
		value = vec2(x, y);
	}

	inline void write_vec2(archive_writer& writer, const char* name, const vec2& value) {
		write_floats(writer, name, value._x, value._y);
	}

	//NOTE: archiving vec2 arrays is a bit clumsy because reader/writers don't support array of arrays. it would severly
	//bloat the interface.
	namespace archiving_vec2_impl {
		class archivable_vec2 : public vec2, public archivable {
		public:
			archivable_vec2() {}
			archivable_vec2(const vec2& copy) : vec2(copy) {}

			virtual void read_from_archive(archive_reader& reader) override {
				read_float(reader, "x", _x);
				read_float(reader, "y", _y);
			}

			virtual void write_to_archive(archive_writer& writer) const override {
				write_float(writer, "x", _x);
				write_float(writer, "y", _y);
			}
		};
	}

	template<typename T> void read_vec2_vector(archive_reader& reader, const char* name, T& values) {
		read_object_vector(reader, name, values, [](archive_reader& reader) {
			archiving_vec2_impl::archivable_vec2 v;
			v.read_from_archive(reader);
			return static_cast<vec2>(v);
		});
	}

	template<typename T> void write_vec2_vector(archive_writer& writer, const char* name, const T& values) {
		write_object_vector(writer, name, values, [](archive_writer& writer, const vec2& value) {
			archiving_vec2_impl::archivable_vec2(value).write_to_archive(writer);
		});
	}

	template<typename T> void read_vec3_no_z_vector(archive_reader& reader, const char* name, T& values) {
		read_object_vector(reader, name, values, [](archive_reader& reader) {
			archiving_vec2_impl::archivable_vec2 v;
			v.read_from_archive(reader);
			return vec3(v._x, v._y, 0.f);
		});
	}

	template<typename T> void write_vec3_no_z_vector(archive_writer& writer, const char* name, const T& values) {
		write_object_vector(writer, name, values, [](archive_writer& writer, const vec3& value) {
			ASSERT(value._z == 0.f);
			archiving_vec2_impl::archivable_vec2(vec2(value._x, value._y)).write_to_archive(writer);
		});
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
