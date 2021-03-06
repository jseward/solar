#pragma once

#include "setting_template.h"
#include "solar/archiving/archiving_enum_helpers.h"

namespace solar {

	template<typename T>
	class enum_setting : public setting_template<typename T> {
	public:
		enum_setting(const char* group_name, const char* name, typename T default_value)
			: setting_template<T>(setting_value_type::ENUM, group_name, name, default_value) {
			}

		virtual checksum get_default_value_checksum() const override {
			return checksum().add_enum<T>(get_default_value());
		}

		virtual void read_from_archive(archive_reader& reader, const char* name) override {
			auto e = T::invalid;
			read_enum_as_string<T>(reader, name, e);
			set_current_value(e);
		}

		virtual void write_to_archive(archive_writer& writer, const char* name) const override {
			write_enum_as_string<T>(writer, name, get_current_value());
		}
	};

}
