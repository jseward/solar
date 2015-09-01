#pragma once

#include "archive_reader.h"
#include "solar/json/json_document.h"

namespace solar {

	class json_archive_reader : public archive_reader {

	private:
		stream& _stream;
		json_document _document;
		json_object* _current_object; //not using std::unique_ptr due to internal trickery when reading object/objects

	public:
		explicit json_archive_reader(stream& stream);
		virtual ~json_archive_reader();

		void attempt_raise_error_for_unused_values() const;

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
		bool try_get_array_of_size(json_array& out, const char* name, unsigned int required_size);

	private:
		class temp_swap_current_object {
		private:
			json_archive_reader* _reader;
			const char* _name;
			json_object* _old_object;
			json_object* _new_object;

		public:
			temp_swap_current_object(json_archive_reader* reader, const char* name, json_object* new_object);
			~temp_swap_current_object();
		};
	};

}