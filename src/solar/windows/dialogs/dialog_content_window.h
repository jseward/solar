#pragma once

#include "solar/windows/window.h"

namespace solar {

	class dialog_window;

	class dialog_content_window : public window {
	private:
		dialog_window& _dialog_window;

	public:
		dialog_content_window(dialog_window& dialog_window);
	};

}