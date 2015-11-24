#pragma once

#include "solar/rendering/cursors/cursor.h"

namespace solar {

	class opengl_cursor : public cursor {
	private:
		point _raw_location;

	public:
		opengl_cursor();
		virtual ~opengl_cursor();

		virtual void set_icon(cursor_icon& icon) override;
		virtual void set_raw_location(const point& location) override;
		virtual point get_hotspot_location() const override;
	};

}