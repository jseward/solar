#pragma once

#include <string>
#include "solar/utility/checksum.h"
#include "solar/archiving/single_value_archivable.h"
#include "setting_value_type.h"

namespace solar {

	class setting : public single_value_archivable {

	private:
		setting_value_type _value_type;
		std::string _group_name;
		std::string _name;

	public:
		setting(setting_value_type value_type, const char* group_name, const char* name);
		virtual ~setting();

		setting_value_type get_value_type() const;
		const std::string& get_group_name() const;
		const std::string& get_name() const;

		virtual checksum get_archive_version_checksum() const = 0;
	};

}