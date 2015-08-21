#include "win32_d3d9_engine.h"
#include "solar/rendering/shaders/shader_id.h"
#include "solar/rendering/textures/texture_id.h"
#include "solar/rendering/meshes/mesh_id.h"
#include "solar/rendering/cursors/cursor_icon_id.h"

namespace solar {

	win32_d3d9_engine::win32_d3d9_engine() 
		: _win32_windowed_app(_win32_file_system)
		, _resource_system(_win32_file_system, _win32_directory_change_watcher)
		, _d3d9_render_device(_d3d9_context)
		, _d3d9_cursor(_d3d9_context)
		, _d3d9_prim2d(_d3d9_context)
		, _d3d9_texture_factory(_d3d9_context, _resource_system)
		, _d3d9_shader_factory(_d3d9_context, _resource_system)
		, _d3d9_mesh_factory(_d3d9_context, _resource_system)
		, _d3d9_mesh_renderer(_d3d9_context)
		, _d3d9_cursor_icon_factory(_d3d9_context, _d3d9_cursor, _resource_system)
		, _setting_registry(_win32_file_system) {

		shader_id::set_factory(&_d3d9_shader_factory);
		texture_id::set_factory(&_d3d9_texture_factory);
		mesh_id::set_factory(&_d3d9_mesh_factory);
		cursor_icon_id::set_factory(&_d3d9_cursor_icon_factory);
	}

	win32_d3d9_engine::~win32_d3d9_engine() {
		shader_id::set_factory(nullptr);
		texture_id::set_factory(nullptr);
		mesh_id::set_factory(nullptr);
		cursor_icon_id::set_factory(nullptr);
	}

	bool win32_d3d9_engine::setup(const win32_d3d9_engine_setup_params& params) {

		std::vector<solar::win32_windowed_app_wnd_proc_handler> wnd_proc_handlers;
		wnd_proc_handlers.push_back([&](HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) { return _d3d9_context.handle_wnd_proc(hwnd, message, wparam, lparam); });
		wnd_proc_handlers.push_back([&](HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) { return _win32_mouse_device.handle_wnd_proc(hwnd, message, wparam, lparam); });
		wnd_proc_handlers.push_back([&](HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) { return _win32_keyboard_device.handle_wnd_proc(hwnd, message, wparam, lparam); });

		if (!_win32_windowed_app.setup(solar::win32_windowed_app_setup_params()
			.set_hinstance(params.get_hinstance())
			.set_idle_proc(params.get_idle_proc())
			.set_wnd_proc_handlers(wnd_proc_handlers)
			.set_window_class_name(params.get_window_class_name())
			.set_window_caption(params.get_window_caption())
			.set_small_icon_id(params.get_small_icon_id())
			.set_large_icon_id(params.get_large_icon_id())
			.set_error_dialog_resources(params.get_error_dialog_resources())
			.set_is_visible_by_default(false) //d3d9_context controls visibility
			)) {

			return false;
		}

		_win32_mouse_device.setup(_win32_windowed_app.get_hwnd());
		
		_win32_file_system.setup(solar::win32_file_system_setup_params()
			.set_user_root_path_company_name(params.get_user_root_path_company_name())
			.set_user_root_path_app_name(params.get_user_root_path_app_name()));

		_win32_windowed_app.open_log_file(
			_win32_file_system.make_user_file_path(
				"logs", 
				build_string("log_{}.txt", _win32_windowed_app.get_processs_uid())));

		add_and_load_all_settings(params);

		_resource_system.setup(resource_system_params()
			.set_is_watching_enabled(true)
			.add_provider(resource_provider().build_as_file_system(_win32_file_system, "..\\")));
			
		if (!_d3d9_context.setup(
			_win32_windowed_app.get_hwnd(),
			solar::d3d9_user_settings()
				.set_window_type(_engine_settings.get_window_type())
			)) {

			return false;
		}

		_d3d9_render_device.setup();
		_d3d9_shader_factory.setup();
		_d3d9_texture_factory.setup();
		_d3d9_mesh_factory.setup();
		_d3d9_mesh_renderer.setup();
		_d3d9_cursor_icon_factory.setup();
		_d3d9_prim2d.setup(solar::d3d9_prim2d_setup_params());

		return true;
	}

	void win32_d3d9_engine::teardown() {
		_d3d9_prim2d.teardown();
		_d3d9_cursor_icon_factory.teardown();
		_d3d9_mesh_renderer.teardown();
		_d3d9_mesh_factory.teardown();
		_d3d9_texture_factory.teardown();
		_d3d9_shader_factory.teardown();
		_d3d9_render_device.teardown();
		_d3d9_context.teardown();
		_resource_system.teardown();
	}

	int win32_d3d9_engine::run() {
		return _win32_windowed_app.run();
	}

	void win32_d3d9_engine::add_and_load_all_settings(const win32_d3d9_engine_setup_params& params) {
		_engine_settings.add_to_setting_registry(_setting_registry);
		if (params.get_add_to_setting_registry_func() != nullptr) {
			params.get_add_to_setting_registry_func()(_setting_registry);
		}
		_setting_registry.load_all_settings();
	}

}
