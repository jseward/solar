#pragma once

#include "solar/strings/string_build.h"
#include "solar/utility/enum_helpers.h"
#include "solar/containers/enum_array.h"
#include "solar/archiving/archivable.h"
#include "solar/archiving/archive_reader.h"
#include "solar/archiving/archive_writer.h"

namespace solar {

	template<typename T> void read_enum_as_string(archive_reader& reader, const char* name, typename T::enum_type& value) {
		std::string value_as_string;
		reader.read_string(name, value_as_string);
		if (!try_parse_enum<T>(value, value_as_string)) {
			reader.raise_error(build_string("Failed to parse enum from string : \"{}\"", value_as_string));
		}
	}

	template<typename T> void write_enum_as_string(archive_writer& writer, const char* name, typename T::enum_type value) {
		writer.write_string(name, T::get_string(value));
	}

	namespace archiving_enum_helpers_impl {

		template<typename ArrayT, typename EnumDetailsT>
		class archivable_enum_array : public archivable {
		private:
			ArrayT* _array;
			const ArrayT* _const_array;

		public:
			archivable_enum_array(ArrayT& arr)
				: _array(&arr)
				, _const_array(nullptr)
			{
			}

			archivable_enum_array(const ArrayT& arr)
				: _array(nullptr)
				, _const_array(&arr)
			{
			}

			virtual void read_from_archive(archive_reader& reader) {
				ASSERT(_array != nullptr);
				FOR_EACH_ENUM(ArrayT::enum_type, e) {
					read_object(reader, EnumDetailsT::get_string(e), _array->at(e));
				}
			}

			virtual void write_to_archive(archive_writer& writer) const {
				ASSERT(_const_array != nullptr);
				FOR_EACH_ENUM(ArrayT::enum_type, e) {
					write_object(writer, EnumDetailsT::get_string(e), _const_array->at(e));
				}
			}
		};
	}

	template<typename ArrayT, typename EnumDetailsT> void read_enum_array(archive_reader& reader, const char* array_name, typename ArrayT& arr) {
		archiving_enum_helpers_impl::archivable_enum_array<ArrayT, EnumDetailsT> archivable_array(arr);
		reader.read_object(array_name, archivable_array);
	}

	template<typename ArrayT, typename EnumDetailsT> void write_enum_array(archive_writer& writer, const char* array_name, typename const ArrayT& arr) {
		archiving_enum_helpers_impl::archivable_enum_array<ArrayT, EnumDetailsT> archivable_array(arr);
		writer.write_object(array_name, archivable_array);
	}

}