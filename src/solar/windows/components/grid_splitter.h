#pragma once

#include "solar/windows/window_component.h"

namespace solar {

	class grid;

	class grid_splitter : public window_component {
	private:
		grid& _grid;

	public:
		grid_splitter(const char* id, grid& grid);
		virtual ~grid_splitter();

		virtual bool on_mouse_button_down(const window_mouse_button_event_params& params) override;
		virtual bool on_mouse_button_up_anywhere(const window_mouse_button_event_params& params) override;
	};

}