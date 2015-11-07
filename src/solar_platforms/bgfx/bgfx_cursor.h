#pragma once

#include "solar/rendering/cursors/cursor.h"

namespace solar {

	class bgfx_cursor : public cursor {
	private:
		point _raw_location;

	public:
		bgfx_cursor();
		virtual ~bgfx_cursor();

		virtual void set_icon(cursor_icon& icon) override;
		virtual void set_raw_location(const point& location) override;
		virtual point get_hotspot_location() const override;
	};

}