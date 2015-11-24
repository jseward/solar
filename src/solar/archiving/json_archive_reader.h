#pragma once

#include "archive_reader.h"
#include "solar/json/json_document.h"

namespace solar {

	class json_archive_reader : public archive_reader {

	private:
		stream& _stream;
		json_document _document;
		json_object* _current_object; //not using std::unique_ptr due to internal trickery when reading object/objects
		const char* _current_object_name;

	public:
		explicit json_archive_reader(stream& stream);
		virtual ~json_archive_reader();

		void attempt_raise_error_for_unused_values() const;

		virtual std::string get_source_description() const override;
		virtual void raise_error(const std::string& error_message) override;
		virtual unsigned int get_read_position() const override;
		virtual void read_object(const char* name, archivable& value) override;
		virtual void read_objects(const char* name, std::function<void(unsigned int)> handle_size_func, std::function<void(archive_reader&, unsigned int)> read_object_func) override;
		virtual void read_bool(const char* name, bool& value) override;
		virtual void read_uint16(const char* name, uint16_t& value) override;
		virtual void read_uint16s_dynamic(const char* name, std::function<void(unsigned int)> handle_size_func, std::function<void(unsigned int, uint16_t)> handle_value_func) override;
		virtual void read_uint16s_fixed(const char* name, unsigned int size, uint16_t* values_begin) override;
		virtual void read_int(const char* name, int& value, const archive_int_compression& compression) override;
		virtual void read_ints_dynamic(const char* name, std::function<void(unsigned int)> handle_size_func, std::function<void(unsigned int, int)> handle_value_func) override;
		virtual void read_ints_fixed(const char* name, unsigned int size, int* values_begin) override;
		virtual void read_optional_int(const char* name, optional<int>& value) override;
		virtual void read_int64(const char* name, int64_t& value) override;
		virtual void read_uint(const char* name, unsigned int& value) override;
		virtual void read_float(const char* name, float& value) override;
		virtual void read_floats_dynamic(const char* name, std::function<void(unsigned int)> handle_size_func, std::function<void(unsigned int, float)> handle_value_func) override;
		virtual void read_floats_fixed(const char* name, unsigned int size, float* values_begin) override;
		virtual void read_string(const char* name, std::string& value) override;
		virtual void read_color(const char* name, color& value) override;

	private:
		bool try_get_array_of_size(json_array& out, const char* name, unsigned int required_size);

	private:
		class temp_swap_current_object {
		private:
			json_archive_reader* _reader;
			const char* _name;
			json_object* _old_object;
			const char* _old_object_name;
			json_object* _new_object;

		public:
			temp_swap_current_object(json_archive_reader* reader, const char* name, json_object* new_object);
			~temp_swap_current_object();
		};
	};

}