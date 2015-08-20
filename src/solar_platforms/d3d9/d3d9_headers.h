#pragma once

//http://stackoverflow.com/questions/11040133/what-does-defining-win32-lean-and-mean-exclude-exactly
#define VC_EXTRALEAN
#define	WIN32_LEAN_AND_MEAN
#define NOMINMAX //window header files define min() / max() as macros, causes conflicts with 3rdParty libs (ex. lua)
#include <windows.h>

#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif

#include "solar_dependencies/directxsdk_june_2010/include/d3d9.h"
#include "solar_dependencies/directxsdk_june_2010/include/d3dx9.h"
#include "solar_dependencies/directxsdk_june_2010/include/dxerr.h"
#include "solar_dependencies/directxsdk_june_2010/include/dxsdkver.h"
#include "solar_dependencies/directxsdk_june_2010/include/dxdiag.h"
