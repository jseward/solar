#pragma once

namespace solar {

	class fixed_time_ticker {

	private:
		const unsigned long _tick_interval_in_ms;
		const unsigned long _max_ticks_per_update;
		const unsigned long _max_queued_time_elapsed_in_ms;

		unsigned long _tick;
		unsigned long _last_time_updated_in_ms;
		unsigned long _queued_time_elapsed_in_ms;

	public:
		fixed_time_ticker(int tick_interval_in_ms, int max_ticks_per_update, int max_queued_time_elapsed_in_ms);

		int update(unsigned long real_time_in_ms);

		unsigned long get_tick() const;
	};

}