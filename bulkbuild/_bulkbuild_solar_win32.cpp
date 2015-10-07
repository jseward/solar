#include "solar_platforms/win32/win32_cli_app.cpp"
#include "solar_platforms/win32/win32_dialog_control.cpp"
#include "solar_platforms/win32/win32_windowed_app.cpp"
#include "solar_platforms/win32/win32_error.cpp"
#include "solar_platforms/win32/win32_error_dialog.cpp"
#include "solar_platforms/win32/win32_string_marshal.cpp"
#include "solar_platforms/win32/win32_debugging.cpp"
#include "solar_platforms/win32/win32_skip_error_registry.cpp"
#include "solar_platforms/win32/win32_file_path_helpers.cpp"
#include "solar_platforms/win32/win32_file_system.cpp"
#include "solar_platforms/win32/win32_file_stream.cpp"
#include "solar_platforms/win32/win32_mouse_device.cpp"
#include "solar_platforms/win32/win32_keyboard_device.cpp"
#include "solar_platforms/win32/win32_real_time.cpp"
#include "solar_platforms/win32/win32_exe_runner.cpp"
#include "solar_platforms/win32/win32_directory_change_watcher.cpp"
#include "solar_platforms/win32/win32_single_directory_change_watcher.cpp"
#include "solar_platforms/win32/win32_unique_handle.cpp"
#include "solar_platforms/win32/win32_thread_helpers.cpp"
#include "solar_platforms/win32/win32_ip_address.cpp"

//must link with:
//shlwapi.lib
//winmm.lib