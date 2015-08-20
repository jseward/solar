#include "time_span.h"
#include "time_convert.h"
#include "solar/strings/string_build.h"

namespace solar {

	namespace time_span_impl {
		const unsigned long MS_PER_SECOND = 1000;
		const unsigned long MS_PER_MINUTE = 60 * MS_PER_SECOND;
		const unsigned long MS_PER_HOUR = 60 * MS_PER_MINUTE;
		const unsigned long MS_PER_DAY = 24 * MS_PER_HOUR;
	}

	time_span::time_span() 
		: _total_ms(0) {
	}

	time_span& time_span::build_from_seconds(float seconds) {
		_total_ms = seconds_to_ms(seconds);
		return *this;
	}

	std::string time_span::to_hours_minutes_seconds_string() {
		auto parts = get_parts();
		return build_string("{:02}:{:02}:{:02}", parts._hours, parts._minutes, parts._seconds);
	}

	time_span::parts time_span::get_parts() const {
		parts parts;

		unsigned long remaining_ms = _total_ms;
		
		parts._days = (int)floor((float)remaining_ms / (float)time_span_impl::MS_PER_DAY);
		remaining_ms -= std::min(remaining_ms, (parts._days * time_span_impl::MS_PER_DAY));

		parts._hours = (int)floor((float)remaining_ms / (float)time_span_impl::MS_PER_HOUR);
		remaining_ms -= std::min(remaining_ms, (parts._hours * time_span_impl::MS_PER_HOUR));

		parts._minutes = (int)floor((float)remaining_ms / (float)time_span_impl::MS_PER_MINUTE);
		remaining_ms -= std::min(remaining_ms, (parts._minutes * time_span_impl::MS_PER_MINUTE));

		parts._seconds = (int)floor((float)remaining_ms / (float)time_span_impl::MS_PER_SECOND);

		return parts;
	}
}
