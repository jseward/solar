#pragma once

#include <string>

namespace solar {

	//A date and time expressed in the number of 100-nanosecond intervals that have elapsed since January 1, 0001 at 00:00:00.000 in the Gregorian calendar.
	class date_time {
	private:
		__int64 _system_time;

	private:
		explicit date_time(__int64 system_time);

	public:
		date_time();

		__int64 get_raw_system_time() const;

		bool operator==(const date_time& rhs) const;
		bool operator!=(const date_time& rhs) const;

		//uses std::strftime formatting rules : http://www.cplusplus.com/reference/ctime/strftime/
		//format may be nullptr (will use default format)
		std::string to_local_time_string(const char* format) const;

		std::string to_iso_8601_string() const;

	public:
		friend date_time make_date_time_from_time_t(time_t t);
		friend date_time make_date_time_as_utc_now();
		friend date_time make_date_time_from_iso_8601_string(const std::string& s);

	private:
		std::string date_time::convert_tm_to_string(const tm& tm, const char* format) const;
	};

}
