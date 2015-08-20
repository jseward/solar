#include "fixed_time_ticker.h"

#include <algorithm>

namespace solar {

	fixed_time_ticker::fixed_time_ticker(int tick_interval_in_ms, int max_ticks_per_update, int max_queued_time_elapsed_in_ms) 
		: _tick_interval_in_ms(tick_interval_in_ms)
		, _max_ticks_per_update(max_ticks_per_update)
		, _max_queued_time_elapsed_in_ms(max_queued_time_elapsed_in_ms)
		
		, _tick(0)
		, _last_time_updated_in_ms(0)
		, _queued_time_elapsed_in_ms(0)
	{
	}

	int fixed_time_ticker::update(unsigned long real_time_in_ms) {
		
		unsigned long elapsed_time_in_ms = 0;
		if (_last_time_updated_in_ms > 0) {
			elapsed_time_in_ms = real_time_in_ms - _last_time_updated_in_ms;
		}
		_last_time_updated_in_ms = real_time_in_ms;

		unsigned int tick_count = 0;

		_queued_time_elapsed_in_ms = std::min(_queued_time_elapsed_in_ms + elapsed_time_in_ms, _max_queued_time_elapsed_in_ms);
		while (_queued_time_elapsed_in_ms >= _tick_interval_in_ms) {
			_queued_time_elapsed_in_ms -= _tick_interval_in_ms;
			tick_count += 1;
			if (tick_count > _max_ticks_per_update) {
				break;
			}
		}

		_tick += tick_count;

		return tick_count;
	}

	unsigned long fixed_time_ticker::get_tick() const {
		return _tick;
	}

}