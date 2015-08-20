#pragma once

namespace solar {

	inline float ms_to_seconds(unsigned long ms) {
		return (float)ms / 1000.f;
	}

	inline unsigned long seconds_to_ms(float seconds) {
		return (unsigned long)(seconds * 1000.f);
	}

}