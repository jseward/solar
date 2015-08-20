#pragma once

#include "alert.h"
#include "solar/strings/string_helpers.h"

#define RETURN_ENUM_STRING(enum_type, enum_value) case enum_type##::enum_value: return #enum_value
#define END_ENUM_STRINGS() default: ALERT("unknown enum value : {0}", (int)e); return "";
#define FOR_EACH_ENUM(enum_type, e) for(enum_type e = (enum_type)(0); e < enum_type::count; e = (enum_type)((int)e + 1))

namespace solar {

	template<typename T> typename T::enum_type parse_enum(const char* s) {
		T::enum_type e = T::enum_type::invalid;
		if (!try_parse_enum<T>(e, s)) {
			ALERT("parse_enum failed : unknown value : {}", s);
		}
		return e;
	}

	template<typename T> typename T::enum_type parse_enum(const std::string& s) {
		return parse_enum<T>(s.c_str());
	}

	template<typename T> bool try_parse_enum(typename T::enum_type& v, const std::string& s) {
		return try_parse_enum<T>(v, s.c_str());
	}

	template<typename T> bool try_parse_enum(typename T::enum_type& v, const char* s) {

		if (is_string_empty(s)) {
			v = T::enum_type::invalid;
			return true;
		}

		FOR_EACH_ENUM(T::enum_type, e) {
			if (are_strings_equal(T::get_string(e), s)) {
				v = e;
				return true;
			}
		}

		return false;
	}

}
