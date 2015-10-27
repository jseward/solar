#pragma once

#include "solar/strings/string_build.h"
#include "solar/utility/enum_helpers.h"
#include "solar/containers/enum_array.h"
#include "solar/archiving/archivable.h"
#include "solar/archiving/archive_reader.h"
#include "solar/archiving/archive_writer.h"
#include "solar/archiving/archive_int_compression.h"

namespace solar {

	template<typename T> void read_enum(archive_reader& reader, const char* name, typename T& value) {
		int value_as_int = 0;
		reader.read_int(name, value_as_int, make_archive_int_compression_range(0, static_cast<int>(T::count) - 1));
		if (value_as_int < 0 || value_as_int >= static_cast<int>(T::count)) {
			reader.raise_error(build_string("Enum is out of range : \"{}\"", value_as_int));
			value = T::invalid;
		}
		value = static_cast<T>(value_as_int);
	}

	template<typename T> void write_enum(archive_writer& writer, const char* name, typename const T& value) {
		const int value_as_int = static_cast<int>(value);
		writer.write_int(name, value_as_int, make_archive_int_compression_range(0, static_cast<int>(T::count) - 1));
	}

	template<typename T> void read_enum_as_string(archive_reader& reader, const char* name, typename T& value) {
		std::string value_as_string;
		reader.read_string(name, value_as_string);
		if (!try_parse_enum<T>(value, value_as_string)) {
			reader.raise_error(build_string("Failed to parse enum from string : \"{}\"", value_as_string));
		}
	}

	template<typename T> void write_enum_as_string(archive_writer& writer, const char* name, typename T value) {
		writer.write_string(name, to_string(value));
	}

	namespace archiving_enum_helpers_impl {

		template<typename ArrayT>
		class archivable_enum_array : public archivable {
		public:
			using read_function = std::function<void(archive_reader&, const char*, typename ArrayT::value_type&)>;
			using write_function = std::function<void(archive_writer&, const char*, const typename ArrayT::value_type&)>;

		private:
			ArrayT* _read_array;
			read_function _read_function;
			const ArrayT* _write_array;
			write_function _write_function;

		public:
			archivable_enum_array(ArrayT& arr, read_function read_function)
				: _read_array(&arr)
				, _read_function(read_function)
				, _write_array(nullptr)
				, _write_function(nullptr) {
			}

			archivable_enum_array(const ArrayT& arr, write_function write_function)
				: _read_array(nullptr)
				, _read_function(nullptr)
				, _write_array(&arr)
				, _write_function(write_function) {
			}

			virtual void read_from_archive(archive_reader& reader) {
				ASSERT(_read_array != nullptr);
				ASSERT(_read_function != nullptr);
				FOR_EACH_ENUM(ArrayT::enum_type, e) {
					_read_function(reader, to_string(e), _read_array->at(e));
				}
			}

			virtual void write_to_archive(archive_writer& writer) const {
				ASSERT(_write_array != nullptr);
				ASSERT(_write_function != nullptr);
				FOR_EACH_ENUM(ArrayT::enum_type, e) {
					_write_function(writer, to_string(e), _write_array->at(e));
				}
			}
		};
	}

	template<typename ArrayT> void read_enum_array_of_objects(archive_reader& reader, const char* array_name, typename ArrayT& arr) {
		auto read_function = [](archive_reader& reader, const char* name, ArrayT::value_type& value) {
			read_object(reader, name, value);
		};
		archiving_enum_helpers_impl::archivable_enum_array<ArrayT> archivable_array(arr, read_function);
		reader.read_object(array_name, archivable_array);
	}

	template<typename ArrayT> void write_enum_array_of_objects(archive_writer& writer, const char* array_name, typename const ArrayT& arr) {
		auto write_function = [](archive_writer& writer, const char* name, const ArrayT::value_type& value) {
			write_object(writer, name, value);
		};
		archiving_enum_helpers_impl::archivable_enum_array<ArrayT> archivable_array(arr, write_function);
		writer.write_object(array_name, archivable_array);
	}

	template<typename ArrayT> void read_enum_array_of_colors(archive_reader& reader, const char* array_name, typename ArrayT& arr) {
		archiving_enum_helpers_impl::archivable_enum_array<ArrayT> archivable_array(arr, read_color);
		reader.read_object(array_name, archivable_array);
	}

	template<typename ArrayT> void write_enum_array_of_colors(archive_writer& writer, const char* array_name, typename const ArrayT& arr) {
		archiving_enum_helpers_impl::archivable_enum_array<ArrayT> archivable_array(arr, write_color);
		writer.write_object(array_name, archivable_array);
	}

}