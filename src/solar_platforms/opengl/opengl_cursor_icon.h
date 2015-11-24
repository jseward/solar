#pragma once

#include "solar/rendering/cursors/cursor_icon.h"

namespace solar {

	class opengl_cursor_icon : public cursor_icon {
	public:
		opengl_cursor_icon();
		virtual ~opengl_cursor_icon();
	};

}