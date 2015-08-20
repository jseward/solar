#pragma once

#include "assert.h"

#ifndef SOLAR__NO_ASSERT

	#define VERIFY(exp) ASSERT(exp)
	#define IF_VERIFY(exp)\
		if (!(exp)) {\
			if (solar::handle_assert_failed(__FILE__, __LINE__, #exp)) {\
				TRIGGER_BREAKPOINT;\
			}\
		}\
		else\

#else

	#define VERIFY(exp) (exp)
	#define IF_VERIFY(exp) if (exp)

#endif
