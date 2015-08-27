#include "json_array.h"
#include "solar/utility/alert.h"
#include "solar/utility/assert.h"
#include "solar/utility/verify.h"
#include "solar/utility/type_convert.h"
#include "solar/strings/string_build.h"

namespace solar {

	json_array::json_array() 
		: _value(rapidjson::kArrayType)
		, _error_handler([](const std::string& s) { ALERT(s.c_str()); }) {
	}

	json_array::json_array(rapidjson::Value& value, json_array_error_handler error_handler)
		: _value(rapidjson::kArrayType)
		, _error_handler(error_handler) {

		IF_VERIFY(value.IsArray()) {
			_value = value;
		}
	}

	json_array::json_array(json_array&& rhs)
		: _value(rapidjson::kArrayType)
		, _error_handler(rhs._error_handler) {

		ASSERT(rhs._value.IsArray());
		_value = rhs._value;
	}

	json_array::~json_array() {
	}

	void json_array::take_ownership(rapidjson::Value& value, json_array_error_handler error_handler) {
		IF_VERIFY(value.IsArray()) {
			_value = value;
		}
		_error_handler = error_handler;
	}

	void json_array::take_ownership(json_array& other) {
		_value = other._value;
		_error_handler = other._error_handler;
	}

	unsigned int json_array::size() const {
		return _value.Size();
	}

	json_object json_array::get_object(unsigned int index) {
		return json_object(_value[index], _error_handler);
	}

	unsigned short json_array::get_ushort(unsigned int index) {
		unsigned short v = 0;
		if (!try_get_ushort(v, index)) {
			_error_handler(build_string("ushort not found at index:{}", index));
		}
		return v;
	}

	int json_array::get_int(unsigned int index) {
		int v = 0;
		if (!try_get_int(v, index)) {
			_error_handler(build_string("int not found at index:{}", index));
		}
		return v;
	}

	float json_array::get_float(unsigned int index) {
		float v = 0;
		if (!try_get_float(v, index)) {
			_error_handler(build_string("float not found at index:{}", index));
		}
		return v;
	}

	const char* json_array::get_string(unsigned int index) {
		const char* s = "";
		if (!try_get_string(s, index)) {
			_error_handler(build_string("string not found at index:{}", index));
		}
		return s;
	}

	bool json_array::try_get_ushort(unsigned short& out, unsigned int index) {
		if (index >= _value.Size()) {
			return false;
		}

		auto& v = _value[index];
		if (!v.IsUint()) {
			return false;
		}

		if (v.GetUint() > std::numeric_limits<unsigned short>::max()) {
			return false;
		}

		out = uint_to_ushort(v.GetUint());
		return true;
	}

	bool json_array::try_get_int(int& out, unsigned int index) {
		if (index >= _value.Size()) {
			return false;
		}

		auto& v = _value[index];
		if (!v.IsInt()) {
			return false;
		}

		out = v.GetInt();
		return true;
	}

	bool json_array::try_get_float(float& out, unsigned int index) {
		if (index >= _value.Size()) {
			return false;
		}

		auto& v = _value[index];
		if (!v.IsNumber()) {
			return false;
		}

		out = double_to_float(v.GetDouble());
		return true;
	}

	bool json_array::try_get_string(const char*& out, unsigned int index) {
		if (index >= _value.Size()) {
			return false;
		}

		auto& v = _value[index];
		if (!v.IsString()) {
			return false;
		}
		
		out = v.GetString();
		return true;
	}

}
