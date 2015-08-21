#pragma once

#include "solar/utility/property.h"
#include "solar/settings/setting_registry.h"
#include "solar/resources/resource_system.h"
#include "solar_platforms/win32/win32_windowed_app.h"
#include "solar_platforms/win32/win32_file_system.h"
#include "solar_platforms/win32/win32_directory_change_watcher.h"
#include "solar_platforms/win32/win32_mouse_device.h"
#include "solar_platforms/win32/win32_keyboard_device.h"
#include "solar_platforms/d3d9/d3d9_context.h"
#include "solar_platforms/d3d9/d3d9_render_device.h"
#include "solar_platforms/d3d9/d3d9_cursor.h"
#include "solar_platforms/d3d9/d3d9_prim2d.h"
#include "solar_platforms/d3d9/d3d9_texture_factory.h"
#include "solar_platforms/d3d9/d3d9_shader_factory.h"
#include "solar_platforms/d3d9/d3d9_mesh_factory.h"
#include "solar_platforms/d3d9/d3d9_mesh_renderer.h"
#include "solar_platforms/d3d9/d3d9_cursor_icon_factory.h"
#include "win32_d3d9_engine_settings.h"

namespace solar {

	struct win32_d3d9_engine_setup_params {
		PROPERTY_BY_COPY(win32_d3d9_engine_setup_params, HINSTANCE, hinstance, nullptr);
		PROPERTY_BY_COPY(win32_d3d9_engine_setup_params, const WCHAR*, window_class_name, nullptr);
		PROPERTY_BY_COPY(win32_d3d9_engine_setup_params, const WCHAR*, window_caption, nullptr);
		PROPERTY_BY_COPY(win32_d3d9_engine_setup_params, size, min_window_size, size(200, 200));
		PROPERTY_BY_COPY(win32_d3d9_engine_setup_params, int, small_icon_id, 0);
		PROPERTY_BY_COPY(win32_d3d9_engine_setup_params, int, large_icon_id, 0);
		PROPERTY_BY_REF(win32_d3d9_engine_setup_params, win32_windowed_app_idle_proc, idle_proc, nullptr);
		PROPERTY_BY_REF(win32_d3d9_engine_setup_params, win32_error_dialog_resources, error_dialog_resources, win32_error_dialog_resources());
		PROPERTY_BY_REF(win32_d3d9_engine_setup_params, std::string, user_root_path_company_name, "");
		PROPERTY_BY_REF(win32_d3d9_engine_setup_params, std::string, user_root_path_app_name, "");
		PROPERTY_BY_COPY(win32_d3d9_engine_setup_params, std::function<void(setting_registry&)>, add_to_setting_registry_func, nullptr);
	};

	class win32_d3d9_engine {
	public:
		win32_file_system _win32_file_system;
		win32_directory_change_watcher _win32_directory_change_watcher;
		win32_windowed_app _win32_windowed_app;
		win32_mouse_device _win32_mouse_device;
		win32_keyboard_device _win32_keyboard_device;
		resource_system _resource_system;
		d3d9_context _d3d9_context;
		d3d9_render_device _d3d9_render_device;
		d3d9_cursor _d3d9_cursor;
		d3d9_prim2d _d3d9_prim2d;
		d3d9_texture_factory _d3d9_texture_factory;
		d3d9_shader_factory _d3d9_shader_factory;
		d3d9_mesh_factory _d3d9_mesh_factory;
		d3d9_mesh_renderer _d3d9_mesh_renderer;
		d3d9_cursor_icon_factory _d3d9_cursor_icon_factory;
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