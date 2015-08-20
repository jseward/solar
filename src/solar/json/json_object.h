#pragma once

#include <memory>
#include <vector>
#include <string>
#include "solar/io/stream.h"
#include "solar/time/date_time.h"
#include "solar/json/internal/rapidjson_headers.h"

namespace solar {

	typedef std::function<void(const std::string&)> json_object_error_handler;

	class json_array;

	class json_object {
	private:
		rapidjson::Value _value;
		bool _should_track_used_values;
		std::vector<std::string> _used_values;
		json_object_error_handler _error_handler;

	public:
		json_object();
		json_object(rapidjson::Value& value, json_object_error_handler error_handler);
		json_object(json_object&& rhs);
		json_object(const json_object&) = delete; //internal rapidjson value has move only semantics on copy
		json_object& operator=(const json_object&) = delete; //internal rapidjson value has move only semantics on copy
		~json_object();
		
		void take_ownership(rapidjson::Value& value, json_object_error_handler error_handler);

		void set_should_track_used_values();
		void raise_error_for_unused_values(const char* object_name);

		//get_object is destructive due to how rapidjson works (moves memory on copy)
		json_object get_object(const char* name);
		json_array get_array(const char* name);
		bool get_bool(const char* name);
		unsigned short get_ushort(const char* name);
		int get_int(const char* name);
		unsigned int get_uint(const char* name);
		float get_float(const char* name);
		std::string get_string(const char* name);
		date_time get_date_time(const char* name);

		bool try_get_object(json_object& out, const char* name);
		bool try_get_array(json_array& out, const char* name);
		bool try_get_bool(bool& out, const char* name);
		bool try_get_ushort(unsigned short& out, const char* name);
		bool try_get_int(int& out, const char* name);
		bool try_get_uint(unsigned int& out, const char* name);
		bool try_get_float(float& out, const char* name);
		bool try_get_string(std::string& out, const char* name);
		bool try_get_date_time(date_time& out, const char* name);
	};

}
