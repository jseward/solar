#pragma once

#include "solar/math/size.h"

namespace solar {

	class texture {
	public:
		virtual size request_size() = 0;
	};

}