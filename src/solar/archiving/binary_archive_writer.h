#pragma once

#include <memory>
#include "solar/io/file_stream.h"
#include "solar/utility/verify.h"
#include "archive_writer.h"

namespace solar {

	class binary_archive_writer : public archive_writer {
	public:
		enum class should_calculate_size { YES, NO };

	private:
		stream* _stream;
		should_calculate_size _should_calculate_size;
		unsigned int _calculated_size;

	public:
		binary_archive_writer(stream& stream);
		binary_archive_writer(should_calculate_size should_calculate_size);
		virtual ~binary_archive_writer();

		const unsigned int get_calculated_size() const;

		virtual void begin_writing() override;
		virtual void end_writing() override;

		virtual void write_name(const char* name) override;
		virtual void write_array(unsigned int size, std::function<void(archive_writer&, unsigned int)> write_element_func) override;
		virtual void write_object(std::function<void(archive_writer&)> write_object_func) override;
		virtual void write_bool(bool value) override;
		virtual void write_uint16(uint16_t value) override;
		virtual void write_int(int value, const archive_int_compression& compression) override;
		virtual void write_int64(int64_t value) override;
		virtual void write_uint(unsigned int value) override;
		virtual void write_float(float value) override;
		virtual void write_string(const std::string& value) override;
		virtual void write_color(const color& value) override;

	private:
		template<typename T> void write_atomic_value(const T& value);
		void write_bytes(const char* data, unsigned int data_size);
	};

	template<typename T> inline void binary_archive_writer::write_atomic_value(const T& value) {
		write_bytes(reinterpret_cast<const char*>(&value), sizeof(T));
	}

}