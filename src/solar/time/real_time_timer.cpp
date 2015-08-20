#include "real_time_timer.h"
#include "real_time.h"
#include "time_convert.h"

namespace solar {

	real_time_timer::real_time_timer() 
		: _start_time_in_ms(get_real_time_in_ms()) {
	}

	void real_time_timer::reset() {
		_start_time_in_ms = get_real_time_in_ms();
	}

	unsigned long real_time_timer::get_time_elapsed_in_ms() const {
		return get_real_time_in_ms() - _start_time_in_ms;
	}

	float real_time_timer::get_time_elapsed_in_seconds() const {
		return ms_to_seconds(get_time_elapsed_in_ms());
	}

}
