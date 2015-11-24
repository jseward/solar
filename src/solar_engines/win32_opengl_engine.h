#pragma once

#include "solar/utility/property.h"
#include "solar/io/file_change_watcher.h"
#include "solar/resources/resource_system.h"
#include "solar/settings/setting_registry.h"
#include "solar/cursors/cursor.h"
#include "solar_platforms/win32/win32_windowed_app.h"
#include "solar_platforms/win32/win32_file_system.h"
#include "solar_platforms/win32/win32_directory_change_watcher.h"
#include "solar_platforms/win32/win32_mouse_device.h"
#include "solar_platforms/win32/win32_keyboard_device.h"
#include "solar_platforms/win32/win32_clipboard.h"
#include "solar_platforms/opengl/opengl_render_device.h"
#include "solar_platforms/opengl/opengl_prim2d.h"
#include "solar_platforms/opengl/opengl_prim2d_lines.h"
#include "solar_platforms/opengl/opengl_texture_factory.h"
#include "solar_platforms/opengl/opengl_shader_program_factory.h"
#include "solar_platforms/opengl/opengl_mesh_factory.h"
#include "solar_platforms/opengl/opengl_font_renderer_shader_program_provider.h"
#include "solar_platforms/opengl/platforms/win32_opengl_context.h"
#include "win32_opengl_engine_settings.h"

namespace solar {

	struct win32_opengl_engine_setup_params {
		MAKE_PROPERTY(win32_opengl_engine_setup_params, win32_windowed_app_setup_params, windowed_app_setup_params, win32_windowed_app_setup_params());
		MAKE_PROPERTY(win32_opengl_engine_setup_params, win32_file_system_setup_params, file_system_setup_params, win32_file_system_setup_params());
	};

	class win32_opengl_engine {
	public:
		win32_clipboard _win32_clipboard;
		win32_file_system _win32_file_system;
		win32_directory_change_watcher _win32_directory_change_watcher;
		win32_windowed_app _win32_windowed_app;
		win32_mouse_device _win32_mouse_device;
		win32_keyboard_device _win32_keyboard_device;
		file_change_watcher _file_change_watcher;
		resource_system _resource_system;
		win32_opengl_context _opengl_context;
		opengl_render_device _opengl_render_device;
		opengl_texture_factory _opengl_texture_factory;
		opengl_shader_program_factory _opengl_shader_program_factory;
		opengl_font_renderer_shader_program_provider _opengl_font_renderer_shader_program_provider;
		opengl_mesh_factory _opengl_mesh_factory;
		opengl_prim2d _opengl_prim2d;
		opengl_prim2d_lines _opengl_prim2d_lines;
		win32_opengl_engine_settings _engine_settings;
		setting_registry _setting_registry;
		cursor _cursor;

	public:
		win32_opengl_engine();
		~win32_opengl_engine();

		bool setup(const win32_opengl_engine_setup_params& params);
		void teardown();
		int run();

	private:
		void add_and_load_all_settings(const win32_opengl_engine_setup_params& params);
	};

}