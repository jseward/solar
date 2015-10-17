#pragma once

#include "solar/windows/window.h"

namespace solar {

	class dialog_window;

	class dialog_close_button : public window {
	private:
		dialog_window& _dialog_window;
	
	public:
		dialog_close_button(dialog_window& dialog_window);
		virtual void on_parent_area_changed() override;
		virtual void render(const window_render_params& params) override;
	};

}