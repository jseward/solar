#pragma once

#include <string>

namespace solar {

	class time_span {
	private:
		unsigned long _total_ms;

	public:
		time_span();
		time_span& build_from_seconds(float seconds);

		int get_hours() const;
		int get_minutes() const;
		int get_seconds() const;

		std::string to_hours_minutes_seconds_string();

	private:
		struct parts {
			int _days;
			int _hours;
			int _minutes;
			int _seconds;
		};
		parts get_parts() const;
	};

}