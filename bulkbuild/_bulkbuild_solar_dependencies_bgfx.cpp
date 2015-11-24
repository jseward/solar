//NOTE: on windows requires psapi.lib

#define _CRT_SECURE_NO_WARNINGS

#pragma warning(push)
#pragma warning(disable:4324) //warning C4324: 'bgfx::Frame': structure was padded due to alignment specifier

#include "solar_dependencies/bgfx/src/amalgamated.cpp"

#pragma warning(pop)