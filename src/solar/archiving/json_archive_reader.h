#pragma once

#include "solar/json/json_document.h"

#include "archive_reader.h"
#include "archive_context_stack.h"

namespace solar {

	class json_archive_reader : public archive_reader {

	private:
		stream& _stream;
		json_document _document;
		json_object _root_object;
		json_object* _current_object;
		json_array* _current_array;
		archive_context_stack _context_stack;

	public:
		explicit json_archive_reader(stream& stream);
		virtual ~json_archive_reader();

		void attempt_raise_error_for_unused_values() const;

		virtual std::string get_source_description() const override;
		virtual void raise_error(const std::string& error_message) const override;
		virtual unsigned int get_read_position() const override;

		virtual void read_name(const char* name) override;
		virtual void read_array(
			std::function<bool(archive_reader&, unsigned int)> handle_size_func, 
			std::function<void(archive_reader&, unsigned int)> read_element_func) override;
		virtual void read_object(std::function<void(archive_reader&)> read_object_func) override;
		virtual void read_bool(bool& value) override;
		virtual void read_uint16(uint16_t& value) override;
		virtual void read_int(int& value, const archive_int_compression& compression) override;
		virtual void read_int64(int64_t& value) override;
		virtual void read_uint(unsigned int& value) override;
		virtual void read_float(float& value) override;
		virtual void read_string(std::string& value) override;
		virtual void read_color(color& value) override;

	private:
		void read_array_internal(
			std::function<bool(archive_reader&, unsigned int)> handle_size_func, 
			std::function<void(archive_reader&, unsigned int)> read_element_func, json_array& new_array);
		void read_object_internal(
			std::function<void(archive_reader&)> read_object_func, 
			json_object& new_object);
		
		template<typename T> void read_atomic_value(
			T& value,
			std::function<T(const json_array& a, unsigned int i)> get_value_from_array_func,
			std::function<T(const json_object& o, const char* n)> get_value_from_object_func) {

			auto frame = _context_stack.pop();
			if (frame.is_array_element()) {
				ASSERT(_current_array != nullptr);
				value = get_value_from_array_func(*_current_array, frame._array_index);
			}
			else {
				ASSERT(frame.is_object_member());				
				ASSERT(_current_object != nullptr);
				value = get_value_from_object_func(*_current_object, frame._member_name);
			}
		}
	};

}