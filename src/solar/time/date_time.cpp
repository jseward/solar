#include "date_time.h"
#include <ctime>
#include "solar/strings/strptime.h"

namespace solar {

	date_time make_date_time_from_time_t(time_t t) {
		return date_time(t);
	}

	date_time make_date_time_as_utc_now() {
		return date_time(::time(0));
	}

	date_time::date_time()
		: _system_time(0) {
	}

	bool date_time::operator==(const date_time& rhs) const {
		return (_system_time == rhs._system_time);
	}

	bool date_time::operator!=(const date_time& rhs) const {
		return !(*this == rhs);
	}

	date_time::date_time(__int64 system_time) 
		: _system_time(system_time) {
	}

	__int64 date_time::get_raw_system_time() const {
		return _system_time;
	}

	std::string date_time::to_local_time_string(const char* format) const {
		static const char* DEFAULT_FORMAT = "%H:%M:%S %m/%d/%Y";
		
		//http://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c
		tm local_tm;
		errno_t err = ::localtime_s(&local_tm, &_system_time);
		if (err != 0) {
			return "???";
		}
		else {
			return convert_tm_to_string(local_tm, format != nullptr ? format : DEFAULT_FORMAT);
		}
	}

	std::string date_time::to_iso_8601_string() const {
		//http://en.cppreference.com/w/cpp/chrono/c/strftime
		//http://raygun.io/raygun-providers/rest-json-api
		//1970-12-31T23:50:59Z
		static const char* FORMAT = "%Y-%m-%dT%H:%M:%SZ";

		tm utc_tm;
		errno_t err = ::gmtime_s(&utc_tm, &_system_time);
		if (err != 0) {
			return "???";
		}
		else {
			return convert_tm_to_string(utc_tm, FORMAT);
		}
	}

	std::string date_time::convert_tm_to_string(const tm& tm, const char* format) const {
		//http://stackoverflow.com/questions/7935483/c-function-to-format-time-t-as-stdstring-buffer-length
		std::string buffer;
		buffer.resize(100);

		int len = strftime(&buffer[0], buffer.size(), format, &tm);
		while (len == 0) {
			buffer.resize(buffer.size() * 2);
			if (buffer.size() > 1000) {
				return "???"; //break out of potentially infinite loop (format is invalid returning len of 0)
			}
			len = strftime(&buffer[0], buffer.size(), format, &tm);
		}
		return buffer;
	}

	date_time make_date_time_from_iso_8601_string(const std::string& s) {
		__int64 system_time = 0;

		if (!s.empty()) {
			tm utc_tm = { 0 };
			strptime(s.c_str(), "%Y-%m-%dT%T%z", &utc_tm);
			system_time = _mkgmtime(&utc_tm);
		}

		return date_time(system_time);;
	}

}