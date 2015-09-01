#pragma once

#include <memory>
#include "solar/io/file_stream.h"
#include "solar/utility/verify.h"
#include "archive_reader.h"

namespace solar {

	class binary_archive_reader : public archive_reader {

	private:
		stream& _stream;

	public:
		binary_archive_reader(stream& stream);
		virtual ~binary_archive_reader();

		virtual std::string get_source_description() const override;
		virtual void raise_error(const std::string& error_message) override;
		virtual void read_object(const char* name, archivable& value) override;
		virtual void read_objects(const char* name, std::function<void(unsigned int)> handle_size_func, std::function<void(archive_reader&, unsigned int)> read_object_func) override;
		virtual void read_bool(const char* name, bool& value) override;
		virtual void read_ushort(const char* name, unsigned short& value) override;
		virtual void read_ushorts(const char* name, unsigned short* begin, unsigned int count) override;
		virtual void read_int(const char* name, int& value) override;
		virtual void read_optional_int(const char* name, optional<int>& value) override;
		virtual void read_ints(const char* name, int* begin, unsigned int count) override;
		virtual void read_uint(const char* name, unsigned int& value) override;
		virtual void read_float(const char* name, float& value) override;
		virtual void read_floats(const char* name, float* begin, unsigned int count) override;
		virtual void read_string(const char* name, std::string& value) override;

	private:
		template<typename T> void read_atomic_value(T& value);
	};

	template<typename T> inline void binary_archive_reader::read_atomic_value(T& value) {
		VERIFY(_stream.read_bytes(reinterpret_cast<char*>(&value), sizeof(T)) == sizeof(T));
	}

}