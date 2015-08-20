#pragma once

#include "solar/utility/alert.h"

#define D3D9_VERIFY(exp) {\
		HRESULT hr = (exp);\
		if (FAILED(hr)) {\
			ALERT("{0} FAILED ({1:x})", #exp, hr); \
		}\
	}