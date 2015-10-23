#pragma once

#include "solar/windows/window.h"

namespace solar {

	class dialog_window;

	class dialog_frame_window : public window {
	private:
		dialog_window& _dialog_window;

	public:
		dialog_frame_window(dialog_window& dialog_window);
		virtual void on_loaded() override;
		virtual void on_parent_area_changed() override;
		virtual void render(const window_render_params& params) override;

	private:
		void update_area();
	};

}