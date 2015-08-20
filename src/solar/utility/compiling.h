#pragma once

#ifdef _WIN32

	#define ANALYSIS_ASSUME(exp) __analysis_assume(exp);

#else

	#error("Uknown Platform")

#endif