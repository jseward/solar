#pragma once

//http://stackoverflow.com/questions/11040133/what-does-defining-win32-lean-and-mean-exclude-exactly
#define VC_EXTRALEAN
#define	WIN32_LEAN_AND_MEAN
#define NOMINMAX //window header files define min() / max() as macros, causes conflicts with 3rdParty libs (ex. lua)

#include <windows.h>
#include <windowsx.h> //GET_X_LPARAM macro
#include <shlobj.h> //SHGetFolderPathW
#include <shlwapi.h> //PathAppend
#include <mmsystem.h> //timeGetTime
#include <commdlg.h> //OPENFILENAMEW