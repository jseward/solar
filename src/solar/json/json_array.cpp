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
		json_object o;
		if (!try_get_object(o, index)) {
			_error_handler(build_string("object not found at index:{}", index));
		}
		return o;
	}

	json_array json_array::get_array(unsigned int index) {
		json_array a;
		if (!try_get_array(a, index)) {
			_error_handler(build_string("array not found at index:{}", index));
		}
		return a;
	}

	bool json_array::get_bool(unsigned int index) const {
		bool v = 0;
		if (!try_get_bool(v, index)) {
			_error_handler(build_string("bool not found at index:{}", index));
		}
		return v;
	}

	uint16_t json_array::get_uint16(unsigned int index) const {
		uint16_t v = 0;
		if (!try_get_uint16(v, index)) {
			_error_handler(build_string("uint16 not found at index:{}", index));
		}
		return v;
	}

	int json_array::get_int(unsigned int index) const {
		int v = 0;
		if (!try_get_int(v, index)) {
			_error_handler(build_string("int not found at index:{}", index));
		}
		return v;
	}

	unsigned int json_array::get_uint(unsigned int index) const {
		unsigned int v = 0;
		if (!try_get_uint(v, index)) {
			_error_handler(build_string("uint not found at index:{}", index));
		}
		return v;
	}

	int64_t json_array::get_int64(unsigned int index) const {
		int64_t v = 0;
		if (!try_get_int64(v, index)) {
			_error_handler(build_string("int64 not found at index:{}", index));
		}
		return v;
	}

	float json_array::get_float(unsigned int index) const {
		float v = 0;
		if (!try_get_float(v, index)) {
			_error_handler(build_string("float not found at index:{}", index));
		}
		return v;
	}

	std::string json_array::get_string(unsigned int index) const {
		std::string s;
		if (!try_get_string(s, index)) {
			_error_handler(build_string("string not found at index:{}", index));
		}
		return s;
	}

	bool json_array::try_get_object(json_object& out, unsigned int index) {
		if (index >= _value.Size()) {
			return false;
		}

		auto& v = _value[index];
		if (!v.IsObject()) {
			return false;
		}

		out.take_ownership(v, _error_handler);
		return true;
	}

	bool json_array::try_get_array(json_array& out, unsigned int index) {
		if (index >= _value.Size()) {
			return false;
		}

		auto& v = _value[index];
		if (!v.IsArray()) {
			return false;
		}

		out.take_ownership(v, _error_handler);
		return true;
	}

	bool json_array::try_get_bool(bool& out, unsigned int index) const {
		if (index >= _value.Size()) {
			return false;
		}

		auto& v = _value[index];
		if (!v.IsBool()) {
			return false;
		}
		
		out = v.GetBool();
		return true;
	}

	bool json_array::try_get_uint16(uint16_t& out, unsigned int index) const {
		if (index >= _value.Size()) {
			return false;
		}

		auto& v = _value[index];
		if (!v.IsUint()) {
			return false;
		}

		if (v.GetUint() > std::numeric_limits<uint16_t>::max()) {
			return false;
		}

		out = uint32_to_uint16(v.GetUint());
		return true;
	}

	bool json_array::try_get_int(int& out, unsigned int index) const {
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

	bool json_array::try_get_uint(unsigned int& out, unsigned int index) const {
		if (index >= _value.Size()) {
			return false;
		}

		auto& v = _value[index];
		if (!v.IsUint()) {
			return false;
		}

		out = v.GetUint();
		return true;
	}

	bool json_array::try_get_int64(int64_t& out, unsigned int index) const {
		if (index >= _value.Size()) {
			return false;
		}

		auto& v = _value[index];
		if (!v.IsInt64()) {
			return false;
		}

		out = v.GetInt64();
		return true;
	}

	bool json_array::try_get_float(float& out, unsigned int index) const {
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

	bool json_array::try_get_string(std::string& out, unsigned int index) const {
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
