#include "setting.h"

namespace solar {

	setting::setting(setting_value_type value_type, const char* group_name, const char* name)
		: _value_type(value_type)
		, _group_name(group_name)
		, _name(name) {
	}

	setting::~setting() {
	}

	setting_value_type setting::get_value_type() const {
		return _value_type;
	}

	const std::string& setting::get_group_name() const {
		return _group_name;
	}

	const std::string& setting::get_name() const {
		return _name;
	}

}