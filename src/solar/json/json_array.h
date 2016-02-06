#pragma once

#include <memory>
#include "solar/io/stream.h"
#include "solar/json/internal/rapidjson_headers.h"
#include "json_object.h"

namespace solar {

	typedef std::function<void(const std::string&)> json_array_error_handler;

	class json_array {
	private:
		rapidjson::Value _value;
		json_array_error_handler _error_handler;

	public:
		json_array();
		json_array(rapidjson::Value& value, json_array_error_handler error_handler);
		json_array(json_array&& rhs);
		json_array(const json_array&) = delete; //internal rapidjson value has move only semantics on copy
		json_array& operator=(const json_array&) = delete; //internal rapidjson value has move only semantics on copy
		~json_array();
		
		void take_ownership(rapidjson::Value& value, json_array_error_handler error_handler);
		void take_ownership(json_array& other);

		unsigned int size() const;
		
		json_object get_object(unsigned int index);
		json_array get_array(unsigned int index);
		bool get_bool(unsigned int index) const;
		uint16_t get_uint16(unsigned int index) const;
		int get_int(unsigned int index) const;
		unsigned int get_uint(unsigned int index) const;
		int64_t get_int64(unsigned int index) const;
		float get_float(unsigned int index) const;
		std::string get_string(unsigned int index) const;

		bool try_get_object(json_object& out, unsigned int index);
		bool try_get_array(json_array& out, unsigned int index);
		bool try_get_bool(bool& out, unsigned int index) const;
		bool try_get_uint16(uint16_t& out, unsigned int index) const;
		bool try_get_int(int& out, unsigned int index) const;
		bool try_get_uint(unsigned int& out, unsigned int index) const;
		bool try_get_int64(int64_t& out, unsigned int index) const;
		bool try_get_float(float& out, unsigned int index) const;
		bool try_get_string(std::string& out, unsigned int index) const;
	};

}
