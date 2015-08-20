#pragma once

namespace solar {

	class real_time_timer {
	private:
		unsigned long _start_time_in_ms;

	public:
		real_time_timer();

		void reset();

		unsigned long get_time_elapsed_in_ms() const;
		float get_time_elapsed_in_seconds() const;
	};

}
