#pragma once

#include "solar/utility/property.h"
#include "solar/settings/setting_registry.h"
#include "solar/io/file_change_watcher.h"
#include "solar/resources/resource_system.h"
#include "solar_platforms/win32/win32_windowed_app.h"
#include "solar_platforms/win32/win32_file_system.h"
#include "solar_platforms/win32/win32_directory_change_watcher.h"
#include "solar_platforms/win32/win32_mouse_device.h"
#include "solar_platforms/win32/win32_keyboard_device.h"
#include "solar_platforms/win32/win32_clipboard.h"
#include "solar_platforms/d3d9/d3d9_context.h"
#include "solar_platforms/d3d9/d3d9_render_device.h"
#include "solar_platforms/d3d9/d3d9_cursor.h"
#include "solar_platforms/d3d9/d3d9_prim2d.h"
#include "solar_platforms/d3d9/d3d9_prim2d_lines.h"
#include "solar_platforms/d3d9/d3d9_texture_factory.h"
#include "solar_platforms/d3d9/d3d9_shader_factory.h"
#include "solar_platforms/d3d9/d3d9_mesh_factory.h"
#include "solar_platforms/d3d9/d3d9_mesh_renderer.h"
#include "solar_platforms/d3d9/d3d9_cursor_icon_factory.h"
#include "win32_d3d9_engine_settings.h"

namespace solar {

	struct win32_d3d9_engine_setup_params {
		MAKE_PROPERTY(win32_d3d9_engine_setup_params, win32_windowed_app_setup_params, windowed_app_setup_params, win32_windowed_app_setup_params());
		MAKE_PROPERTY(win32_d3d9_engine_setup_params, win32_file_system_setup_params, file_system_setup_params, win32_file_system_setup_params());
		MAKE_PROPERTY(win32_d3d9_engine_setup_params, std::function<void(setting_registry&)>, add_settings_func, nullptr);
	};

	class win32_d3d9_engine {
	public:
		win32_clipboard _win32_clipboard;
		win32_file_system _win32_file_system;
		win32_directory_change_watcher _win32_directory_change_watcher;
		win32_windowed_app _win32_windowed_app;
		win32_mouse_device _win32_mouse_device;
		win32_keyboard_device _win32_keyboard_device;
		file_change_watcher _file_change_watcher;
		resource_system _resource_system; //needed by many d3d9 components
		d3d9_context _d3d9_context;
		d3d9_render_device _d3d9_render_device;
		d3d9_cursor _d3d9_cursor;
		d3d9_texture_factory _d3d9_texture_factory;
		d3d9_shader_factory _d3d9_shader_factory;
		d3d9_mesh_factory _d3d9_mesh_factory;
		d3d9_mesh_renderer _d3d9_mesh_renderer;
		d3d9_cursor_icon_factory _d3d9_cursor_icon_factory;
		d3d9_prim2d _d3d9_prim2d;
		d3d9_prim2d_lines _d3d9_prim2d_lines;
		win32_d3d9_engine_settings _engine_settings;
		setting_registry _setting_registry;

	public:
		win32_d3d9_engine();
		~win32_d3d9_engine();

		bool setup(const win32_d3d9_engine_setup_params& params);
		void teardown();
		int run();

	private:
		void add_and_load_all_settings(const win32_d3d9_engine_setup_params& params);
		void remove_all_settings();
	};

}