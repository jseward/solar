#pragma once

namespace solar {

	//tough call on if access to real time should be through a real_time object interface that has platform specific implementations instead of just
	//a simple global function. went for global function this time.

	//PLATFORM DEPENDENT
	extern unsigned long get_real_time_in_ms();

	extern float get_real_time_in_seconds();

}
