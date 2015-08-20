#pragma once

#include <functional>

#include "solar/utility/debugging.h"
#include "solar/utility/compiling.h"

namespace solar{
	using assert_failed_handler = std::function<bool(const char* file_name, int line_number, const char* expression)>;

	void set_assert_failed_handler(assert_failed_handler handler);
	bool handle_assert_failed(const char* file_name, int line_number, const char* expression);
}

#undef ASSERT

#ifndef SOLAR__NO_ASSERT

	//ASSERT(false) => warning C4127: conditional expression is constant
	#define ASSERT(exp) \
		{\
			__pragma(warning(push)) \
			__pragma(warning(disable:4127)) \
			if (!(exp)) { \
			__pragma(warning(pop)) \
				if (solar::handle_assert_failed(__FILE__, __LINE__, #exp)) { \
					TRIGGER_BREAKPOINT; \
				} \
			} \
		}\
		ANALYSIS_ASSUME(exp);
	
#else

	#define ASSERT(exp) ANALYSIS_ASSUME(exp);

#endif
