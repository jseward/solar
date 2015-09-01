#include "grid_splitter.h"

namespace solar {

	grid_splitter::grid_splitter(const char* id, grid& grid)
		: window_component(id) 
		, _grid(grid) {
	}

	grid_splitter::~grid_splitter() {
	}

	bool grid_splitter::on_mouse_button_down(const window_mouse_button_event_params& params) {
		return false;//todo
	}

	bool grid_splitter::on_mouse_button_up_anywhere(const window_mouse_button_event_params& params) {
		return false;//todo

	}

}
