#include "json_object.h"
#include "solar/utility/alert.h"
#include "solar/utility/assert.h"
#include "solar/utility/verify.h"
#include "solar/containers/container_helpers.h"
#include "solar/strings/string_build.h"
#include "solar/strings/string_join.h"
#include "json_array.h"

namespace solar {

	json_object::json_object() 
		: _value(rapidjson::kObjectType)
		, _should_track_used_values(false)
		, _error_handler([](const std::string& s) { ALERT(s.c_str()); }) {
	}

	json_object::json_object(rapidjson::Value& value, json_object_error_handler error_handler)
		: _value(rapidjson::kObjectType)
		, _should_track_used_values(false)
		, _error_handler(error_handler) {

		IF_VERIFY(value.IsObject()) {
			_value = value;
		}
	}

	json_object::json_object(json_object&& rhs) 
		: _value(rapidjson::kObjectType)
		, _should_track_used_values(rhs._should_track_used_values)
		, _error_handler(rhs._error_handler) {

		ASSERT(rhs._value.IsObject());
		_value = rhs._value;
	}

	json_object::~json_object() {
	}

	void json_object::take_ownership(rapidjson::Value& value, json_object_error_handler error_handler) {
		IF_VERIFY(value.IsObject()) {
			_value = value;
		}
		_used_values.clear();
		_error_handler = error_handler;
	}

	void json_object::set_should_track_used_values() {
		_should_track_used_values = true;
	}

	void json_object::raise_error_for_unused_values() {
		ASSERT(_should_track_used_values);

		std::vector<const char*> unused_names;
		for (auto iter = _value.MemberBegin(); iter != _value.MemberEnd(); ++iter) {
			if (!any(_used_values, iter->name.GetString())) {
				unused_names.push_back(iter->name.GetString());
			}
		}

		if (!unused_names.empty()) {
			_error_handler(build_string("unused values detected : [ {} ]", join_strings(", ", unused_names)));
		}
	}

	json_object json_object::get_object(const char* name) {
		json_object o;
		if (!try_get_object(o, name)) {
			_error_handler(build_string("object not found : {}", name));
		}
		return o;
	}

	json_array json_object::get_array(const char* name) {
		json_array a;
		if (!try_get_array(a, name)) {
			_error_handler(build_string("array not found : {}", name));
		}
		return a;
	}

	bool json_object::get_bool(const char* name) const {
		bool v = false;
		if (!try_get_bool(v, name)) {
			_error_handler(build_string("bool not found : {}", name));
		}
		return v;
	}

	uint16_t json_object::get_uint16(const char* name) const {
		uint16_t v = 0;
		if (!try_get_uint16(v, name)) {
			_error_handler(build_string("uint16 not found : {}", name));
		}
		return v;
	}

	int json_object::get_int(const char* name) const {
		int v = 0;
		if (!try_get_int(v, name)) {
			_error_handler(build_string("int not found : {}", name));
		}
		return v;
	}

	unsigned int json_object::get_uint(const char* name) const {
		unsigned int v = 0;
		if (!try_get_uint(v, name)) {
			_error_handler(build_string("uint not found : {}", name));
		}
		return v;
	}

	int64_t json_object::get_int64(const char* name) const {
		int64_t v = 0;
		if (!try_get_int64(v, name)) {
			_error_handler(build_string("int64 not found : {}", name));
		}
		return v;
	}

	float json_object::get_float(const char* name) const {
		float v = 0;
		if (!try_get_float(v, name)) {
			_error_handler(build_string("float not found : {}", name));
		}
		return v;
	}

	std::string json_object::get_string(const char* name) const {
		std::string v;
		if (!try_get_string(v, name)) {
			_error_handler(build_string("string not found : {}", name));
		}
		return v;
	}

	date_time json_object::get_date_time(const char* name) const {
		date_time v;
		if (!try_get_date_time(v, name)) {
			_error_handler(build_string("date_time not found : {}", name));
		}
		return v;
	}

	bool json_object::try_get_object(json_object& out, const char* name) {
		if (!_value.HasMember(name)) {
			return false;
		}

		auto& v = _value[name];
		if (!v.IsObject()) {
			return false;
		}

		if (_should_track_used_values) {
			_used_values.emplace_back(name);
		}

		out.take_ownership(v, _error_handler);
		return true;
	}

	bool json_object::try_get_bool(bool& out, const char* name) const {
		if (!_value.HasMember(name)) {
			return false;
		}

		auto& v = _value[name];
		if (!v.IsBool()) {
			return false;
		}

		if (_should_track_used_values) {
			_used_values.emplace_back(name);
		}

		out = v.GetBool();
		return true;
	}

	bool json_object::try_get_uint16(uint16_t& out, const char* name) const {
		if (!_value.HasMember(name)) {
			return false;
		}

		auto& v = _value[name];
		if (!v.IsUint()) {
			return false;
		}

		if (v.GetUint() > std::numeric_limits<uint16_t>::max()) {
			return false;
		}

		if (_should_track_used_values) {
			_used_values.emplace_back(name);
		}

		out = v.GetUint();
		return true;
	}

	bool json_object::try_get_int(int& out, const char* name) const {
		if (!_value.HasMember(name)) {
			return false;
		}

		auto& v = _value[name];
		if (!v.IsInt()) {
			return false;
		}

		if (_should_track_used_values) {
			_used_values.emplace_back(name);
		}

		out = v.GetInt();
		return true;
	}

	bool json_object::try_get_uint(unsigned int& out, const char* name) const {
		if (!_value.HasMember(name)) {
			return false;
		}

		auto& v = _value[name];
		if (!v.IsUint()) {
			return false;
		}

		if (_should_track_used_values) {
			_used_values.emplace_back(name);
		}

		out = v.GetUint();
		return true;
	}

	bool json_object::try_get_int64(int64_t& out, const char* name) const {
		if (!_value.HasMember(name)) {
			return false;
		}

		auto& v = _value[name];
		if (!v.IsInt64()) {
			return false;
		}

		if (_should_track_used_values) {
			_used_values.emplace_back(name);
		}

		out = v.GetInt64();
		return true;
	}

	bool json_object::try_get_float(float& out, const char* name) const {
		if (!_value.HasMember(name)) {
			return false;
		}

		auto& v = _value[name];
		if (!v.IsNumber()) {
			return false;
		}

		if (_should_track_used_values) {
			_used_values.emplace_back(name);
		}

		out = static_cast<float>(v.GetDouble());
		return true;
	}

	bool json_object::try_get_string(std::string& out, const char* name) const {
		if (!_value.HasMember(name)) {
			return false;
		}

		auto& v = _value[name];
		if (!v.IsString()) {
			return false;
		}

		if (_should_track_used_values) {
			_used_values.emplace_back(name);
		}

		out = std::string(v.GetString(), v.GetStringLength());
		return true;
	}

	bool json_object::try_get_date_time(date_time& out, const char* name) const {
		std::string s;
		if (!try_get_string(s, name)) {
			return false;
		}

		out = make_date_time_from_iso_8601_string(s);
		return true;
	}

	bool json_object::try_get_array(json_array& out, const char* name) {
		if (!_value.HasMember(name)) {
			return false;
		}

		auto& v = _value[name];
		if (!v.IsArray()) {
			return false;
		}

		if (_should_track_used_values) {
			_used_values.emplace_back(name);
		}

		out.take_ownership(v, _error_handler);
		return true;
	}

}
