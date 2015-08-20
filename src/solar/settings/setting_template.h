#pragma once

#include "setting.h"

namespace solar {

	template<typename T>
	class setting_template : public setting {
	private:
		T _default_value;
		T _current_value;

	public:
		setting_template(setting_value_type value_type, const char* group_name, const char* name, const T& default_value);
		const T& get_default_value() const;
		const T& get_current_value() const;
		void set_current_value(const T& value);
		virtual checksum get_archive_version_checksum() const override;
		virtual checksum get_default_value_checksum() const = 0;
	};

	template<typename T>
	inline setting_template<T>::setting_template(setting_value_type value_type, const char* group_name, const char* name, const T& default_value) 
		: setting(value_type, group_name, name)
		, _default_value(default_value)
		, _current_value(default_value) {
	};

	template<typename T>
	inline const T& setting_template<T>::get_default_value() const {
		return _default_value;
	}

	template<typename T>
	inline const T& setting_template<T>::get_current_value() const {
		return _current_value;
	}

	template<typename T>
	inline void setting_template<T>::set_current_value(const T& value) {
		_current_value = value;
	}

	template<typename T>
	inline checksum setting_template<T>::get_archive_version_checksum() const {
		return checksum()
			.add_enum(get_value_type())
			.add_string(get_group_name())
			.add_string(get_name())
			.add_checksum(get_default_value_checksum());
	}

}
