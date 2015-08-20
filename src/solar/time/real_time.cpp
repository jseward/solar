#include "real_time.h"

#include "time_convert.h"

namespace solar {

	float get_real_time_in_seconds() {
		return ms_to_seconds(get_real_time_in_ms());
	}

}