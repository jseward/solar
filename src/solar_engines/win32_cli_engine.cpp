#include "win32_cli_engine.h"

#include <conio.h>
#include "solar/utility/alert.h"
#include "solar/utility/assert.h"
#include "solar/utility/trace.h"

namespace solar {

	win32_cli_engine::win32_cli_engine() {
	}

	win32_cli_engine::~win32_cli_engine() {
	}

	bool win32_cli_engine::setup(const win32_cli_engine_setup_params& params) {

		_win32_cli_app.setup(params.get_app_params());

		_win32_file_system.setup(win32_file_system_setup_params()
			.set_user_file_location(win32_user_file_location::WORKING_DIR));

		return true;
	}

	void win32_cli_engine::teardown() {
	}

	void win32_cli_engine::wait_for_user_input() {
		//http://stackoverflow.com/questions/1449324/how-to-simulate-press-any-key-to-continue
		_getch();
	}
}
