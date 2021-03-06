#include "win32_d3d9_engine.h"

#include "solar/containers/container_helpers.h"
#include "solar/rendering/shaders/shader_program_id.h"
#include "solar/rendering/textures/texture_id.h"
#include "solar/rendering/meshes/mesh_id.h"

namespace solar {

	win32_d3d9_engine::win32_d3d9_engine() 
		: _win32_windowed_app(_win32_file_system)
		, _file_change_watcher(_win32_directory_change_watcher)
		, _resource_system(_win32_file_system, _file_change_watcher)
		, _d3d9_render_device(_d3d9_context)
		, _d3d9_texture_factory(_d3d9_context, _resource_system)
		, _d3d9_shader_program_factory(_d3d9_context, _resource_system)
		, _d3d9_font_renderer_shader_program_provider(_d3d9_shader_program_factory)
		, _d3d9_mesh_factory(_d3d9_context, _resource_system)
		, _d3d9_mesh_renderer(_d3d9_context)
		, _d3d9_prim2d(_d3d9_context, _d3d9_shader_program_factory)
		, _d3d9_prim2d_lines(_d3d9_context)
		, _setting_registry(_win32_file_system, _file_change_watcher) {

		shader_program_id::set_factory(&_d3d9_shader_program_factory);
		mesh_id::set_factory(&_d3d9_mesh_factory);
	}

	win32_d3d9_engine::~win32_d3d9_engine() {
		shader_program_id::set_factory(nullptr);
		mesh_id::set_factory(nullptr);
	}

	bool win32_d3d9_engine::setup(const win32_d3d9_engine_setup_params& params) {

		win32_windowed_app_setup_params adjusted_wasp = params.get_windowed_app_setup_params();
		adjusted_wasp.get_wnd_proc_handlers().push_back([&](HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) { return _d3d9_context.handle_wnd_proc(hwnd, message, wparam, lparam); });
		adjusted_wasp.get_wnd_proc_handlers().push_back([&](HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) { return _win32_mouse_device.handle_wnd_proc(hwnd, message, wparam, lparam); });
		adjusted_wasp.get_wnd_proc_handlers().push_back([&](HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) { return _win32_keyboard_device.handle_wnd_proc(hwnd, message, wparam, lparam); });
		adjusted_wasp.set_is_visible_by_default(false); //d3d9_context controls visibility

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

		if (!_d3d9_context.setup(
			_win32_windowed_app.get_hwnd(),
			solar::d3d9_user_settings()
				.set_window_type(_engine_settings._window_type->get_current_value())
				.set_backbuffer_size(_engine_settings._window_size->get_current_value())
			)) {

			return false;
		}

		_d3d9_render_device.setup();
		_d3d9_shader_program_factory.setup();
		_d3d9_font_renderer_shader_program_provider.setup();
		_d3d9_texture_factory.setup();
		_d3d9_mesh_factory.setup();
		_d3d9_mesh_renderer.setup();
		_d3d9_prim2d.setup(solar::d3d9_prim2d_setup_params());
		_d3d9_prim2d_lines.setup();

		return true;
	}

	void win32_d3d9_engine::teardown() {
		_d3d9_prim2d_lines.teardown();
		_d3d9_prim2d.teardown();
		_d3d9_mesh_renderer.teardown();
		_d3d9_mesh_factory.teardown();
		_d3d9_texture_factory.teardown();
		_d3d9_font_renderer_shader_program_provider.teardown();
		_d3d9_shader_program_factory.teardown();
		_d3d9_render_device.teardown();
		_d3d9_context.teardown();
		_setting_registry.teardown();
		_file_change_watcher.teardown();
		_resource_system.teardown();
	}

	int win32_d3d9_engine::run() {
		return _win32_windowed_app.run();
	}

	void win32_d3d9_engine::add_and_load_all_settings(const win32_d3d9_engine_setup_params& params) {
		UNUSED_PARAMETER(params);
		_engine_settings.add_to_setting_registry(_setting_registry);
		if (params.get_add_settings_func() != nullptr) {
			params.get_add_settings_func()(_setting_registry);
		}
		_setting_registry.load_all_settings();
	}

}
