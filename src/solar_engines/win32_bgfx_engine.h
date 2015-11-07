#pragma once

#include "solar/utility/property.h"
#include "solar/io/file_change_watcher.h"
#include "solar/resources/resource_system.h"
#include "solar/settings/setting_registry.h"
#include "solar_platforms/win32/win32_windowed_app.h"
#include "solar_platforms/win32/win32_file_system.h"
#include "solar_platforms/win32/win32_directory_change_watcher.h"
#include "solar_platforms/win32/win32_mouse_device.h"
#include "solar_platforms/win32/win32_keyboard_device.h"
#include "solar_platforms/win32/win32_clipboard.h"
#include "solar_platforms/bgfx/bgfx_win32_render_device.h"
#include "solar_platforms/bgfx/bgfx_cursor.h"
#include "solar_platforms/bgfx/bgfx_prim2d.h"
#include "solar_platforms/bgfx/bgfx_prim2d_lines.h"
#include "solar_platforms/bgfx/bgfx_texture_factory.h"
#include "solar_platforms/bgfx/bgfx_shader_factory.h"
#include "solar_platforms/bgfx/bgfx_mesh_factory.h"
#include "solar_platforms/bgfx/bgfx_cursor_icon_factory.h"
#include "win32_bgfx_engine_settings.h"

namespace solar {

	struct win32_bgfx_engine_setup_params {
		MAKE_PROPERTY(win32_bgfx_engine_setup_params, win32_windowed_app_setup_params, windowed_app_setup_params, win32_windowed_app_setup_params());
		MAKE_PROPERTY(win32_bgfx_engine_setup_params, win32_file_system_setup_params, file_system_setup_params, win32_file_system_setup_params());
	};

	class win32_bgfx_engine {
	public:
		win32_clipboard _win32_clipboard;
		win32_file_system _win32_file_system;
		win32_directory_change_watcher _win32_directory_change_watcher;
		win32_windowed_app _win32_windowed_app;
		win32_mouse_device _win32_mouse_device;
		win32_keyboard_device _win32_keyboard_device;
		file_change_watcher _file_change_watcher;
		resource_system _resource_system; //needed by many d3d9 components
		bgfx_win32_render_device _bgfx_win32_render_device;
		bgfx_cursor _bgfx_cursor;
		bgfx_texture_factory _bgfx_texture_factory;
		bgfx_shader_factory _bgfx_shader_factory;
		bgfx_mesh_factory _bgfx_mesh_factory;
		bgfx_cursor_icon_factory _bgfx_cursor_icon_factory;
		bgfx_prim2d _bgfx_prim2d;
		bgfx_prim2d_lines _bgfx_prim2d_lines;
		win32_bgfx_engine_settings _engine_settings;
		setting_registry _setting_registry;

	public:
		win32_bgfx_engine();
		~win32_bgfx_engine();

		bool setup(const win32_bgfx_engine_setup_params& params);
		void teardown();
		int run();

	private:
		void add_and_load_all_settings(const win32_bgfx_engine_setup_params& params);
	};

}