#pragma once

#include "solar/utility/property.h"
#include "solar_platforms/win32/win32_file_system.h"
#include "solar_platforms/win32/win32_cli_app.h"
#include "solar_platforms/win32/win32_exe_runner.h"
#include "solar_platforms/win32/win32_directory_change_watcher.h"

namespace solar {

	struct win32_cli_engine_setup_params {
		MAKE_PROPERTY(win32_cli_engine_setup_params, win32_cli_app_setup_params, app_params, win32_cli_app_setup_params());
	};

	class win32_cli_engine {
	public:
		win32_cli_app _win32_cli_app;
		win32_file_system _win32_file_system;
		win32_directory_change_watcher _win32_directory_change_watcher;
		win32_exe_runner _win32_exe_runner;

	public:
		win32_cli_engine();
		~win32_cli_engine();

		bool setup(const win32_cli_engine_setup_params& params);
		void teardown();
		void wait_for_user_input();

	private:
		win32_cli_engine(const win32_cli_engine&);//not copyable
		win32_cli_engine& operator=(win32_cli_engine&);//not copyable
	};

}