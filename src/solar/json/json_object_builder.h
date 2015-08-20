#pragma once

#include "solar/json/internal/rapidjson_headers.h"
#include "solar/utility/assert.h"
#include "json_value_builder.h"

namespace solar {

	template<typename AllocatorT>
	class json_object_builder : public json_value_builder<AllocatorT> {
	public:
		json_object_builder(AllocatorT& allocator)
			: json_value_builder(allocator, rapidjson::kObjectType) {
		}

		json_object_builder(const json_object_builder&) = delete;
		json_object_builder& operator=(const json_object_builder&) = delete;

		json_object_builder& add_int(const char* name, int i) {
			get_value().AddMember(rapidjson::StringRef(name), rapidjson::Value().SetInt(i), get_allocator());
			return *this;
		}

		json_object_builder& add_string(const char* name, const std::string& str) {
			rapidjson::Value str_value;
			str_value.SetString(str.c_str(), get_allocator());
			get_value().AddMember(rapidjson::StringRef(name), str_value, get_allocator());
			return *this;
		}

		json_object_builder& add_date_time(const char* name, const date_time& dt) {
			rapidjson::Value str_value;
			str_value.SetString(dt.to_iso_8601_string().c_str(), get_allocator());
			get_value().AddMember(rapidjson::StringRef(name), str_value, get_allocator());
			return *this;
		}

		json_object_builder& add_array(const char* name, json_value_builder<AllocatorT>& array_builder) {
			ASSERT(array_builder.get_value().GetType() == rapidjson::kArrayType);
			get_value().AddMember(rapidjson::StringRef(name), array_builder.get_value(), get_allocator());
			return *this;
		}
	};

}
