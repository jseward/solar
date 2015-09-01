#pragma once

#include <memory>
#include "solar/io/file_stream.h"
#include "solar/utility/verify.h"
#include "archive_writer.h"

namespace solar {

	class binary_archive_writer : public archive_writer {

	private:
		stream& _stream;

	public:
		binary_archive_writer(stream& stream);
		virtual ~binary_archive_writer();

		virtual void begin_writing() override;
		virtual void end_writing() override;
		virtual void write_object(const char* name, const archivable& value) override;
		virtual void write_objects(const char* name, unsigned int size, std::function<void(archive_writer&, unsigned int)> write_object_func) override;
		virtual void write_bool(const char* name, bool value) override;
		virtual void write_ushort(const char* name, unsigned short value) override;
		virtual void write_ushorts(const char* name, const unsigned short* begin, unsigned int count) override;
		virtual void write_int(const char* name, int value) override;
		virtual void write_optional_int(const char* name, const optional<int>& value);
		virtual void write_ints(const char* name, const int* begin, unsigned int count) override;
		virtual void write_uint(const char* name, unsigned int value) override;
		virtual void write_float(const char* name, float value) override;
		virtual void write_floats(const char* name, const float* begin, unsigned int count) override;
		virtual void write_string(const char* name, const std::string& value) override;

	private:
		template<typename T> void write_atomic_value(const T& value);
	};

	template<typename T> inline void binary_archive_writer::write_atomic_value(const T& value) {
		_stream.write_bytes(reinterpret_cast<const char*>(&value), sizeof(T));
	}

}