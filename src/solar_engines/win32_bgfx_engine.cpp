#include "win32_bgfx_engine.h"

#include "solar/utility/unused_parameter.h"
#include "solar/rendering/shaders/shader_program_id.h"
#include "solar/rendering/textures/texture_id.h"
#include "solar/rendering/meshes/mesh_id.h"
#include "solar/rendering/cursors/cursor_icon_id.h"

namespace solar {

	win32_bgfx_engine::win32_bgfx_engine()
		: _win32_windowed_app(_win32_file_system)
		, _file_change_watcher(_win32_directory_change_watcher)
		, _resource_system(_win32_file_system, _file_change_watcher)
		, _bgfx_context()
		, _bgfx_render_device(_bgfx_context)
		, _bgfx_cursor()
		, _bgfx_texture_factory(_resource_system)
		, _bgfx_shader_program_factory(_resource_system)
		, _bgfx_mesh_factory(_resource_system)
		, _bgfx_cursor_icon_factory(_bgfx_cursor, _resource_system)
		, _bgfx_prim2d()
		, _bgfx_prim2d_lines() 
		, _setting_registry(_win32_file_system, _file_change_watcher) {

		shader_program_id::set_factory(&_bgfx_shader_program_factory);
		texture_id::set_factory(&_bgfx_texture_factory);
		mesh_id::set_factory(&_bgfx_mesh_factory);
		cursor_icon_id::set_factory(&_bgfx_cursor_icon_factory);
	}

	win32_bgfx_engine::~win32_bgfx_engine() {
		shader_program_id::set_factory(nullptr);
		texture_id::set_factory(nullptr);
		mesh_id::set_factory(nullptr);
		cursor_icon_id::set_factory(nullptr);
	}

	bool win32_bgfx_engine::setup(const win32_bgfx_engine_setup_params& params) {

		win32_windowed_app_setup_params adjusted_wasp = params.get_windowed_app_setup_params();
		adjusted_wasp.get_wnd_proc_handlers().push_back([&](HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) { return _bgfx_context.handle_wnd_proc(hwnd, message, wparam, lparam); });
		adjusted_wasp.get_wnd_proc_handlers().push_back([&](HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) { return _win32_mouse_device.handle_wnd_proc(hwnd, message, wparam, lparam); });
		adjusted_wasp.get_wnd_proc_handlers().push_back([&](HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) { return _win32_keyboard_device.handle_wnd_proc(hwnd, message, wparam, lparam); });
		adjusted_wasp.set_is_visible_by_default(true);

		if (!_win32_windowed_app.setup(adjusted_wasp)) {
			return false;
		}

		_win32_clipboard.setup(_win32_windowed_app.get_hwnd());
		_win32_mouse_device.setup(_win32_windowed_app.get_hwnd());
		_win32_file_system.setup(params.get_file_system_setup_params());

		_win32_windowed_app.open_log_file(
			_win32_file_system.make_user_file_path(
				"logs",
				build_string("log_{}.txt", _win32_windowed_app.get_processs_uid())));

		auto bin_root_path = _win32_file_system.resolve_file_path("..\\");

		//NOTE: file change watcher must come before settings and other systems are setup as they have files to watch.
		#ifndef SOLAR__NO_FILE_CHANGE_WATCHER
		{
			_file_change_watcher.setup({
				bin_root_path,
				_setting_registry.get_root_dir_path() });
		}
		#endif

		add_and_load_all_settings(params);

		_resource_system.setup(resource_system_params()
			.set_is_watching_enabled(true)
			.add_provider(resource_provider().build_as_file_system(_win32_file_system, bin_root_path)));

		if (!_bgfx_context.setup(_win32_windowed_app.get_hwnd(), bgfx_user_settings())) {
			return false;
		}

		_bgfx_shader_program_factory.setup();
		_bgfx_texture_factory.setup();
		_bgfx_mesh_factory.setup();
		_bgfx_cursor_icon_factory.setup();
		_bgfx_prim2d.setup();
		_bgfx_prim2d_lines.setup();

		return true;
	}

	void win32_bgfx_engine::teardown() {
		
		_bgfx_prim2d_lines.teardown();
		_bgfx_prim2d.teardown();
		_bgfx_cursor_icon_factory.teardown();
		_bgfx_mesh_factory.teardown();
		_bgfx_texture_factory.teardown();
		_bgfx_shader_program_factory.teardown();

		_bgfx_context.teardown();

		_setting_registry.teardown();
		_file_change_watcher.teardown();
		_resource_system.teardown();
	}

	int win32_bgfx_engine::run() {
		return _win32_windowed_app.run();
	}

	void win32_bgfx_engine::add_and_load_all_settings(const win32_bgfx_engine_setup_params& params) {
		UNUSED_PARAMETER(params);
		_engine_settings.add_to_setting_registry(_setting_registry);
		_setting_registry.load_all_settings();
	}

}
