#pragma once

#include "solar/rendering/cursors/cursor.h"

namespace solar {

	class d3d9_context;
	class d3d9_cursor_icon;

	class d3d9_cursor : public cursor {
	private:
		d3d9_context& _context;
		const d3d9_cursor_icon* _active_icon;
		point _raw_location;

	public:
		d3d9_cursor(d3d9_context& context);
		virtual ~d3d9_cursor();

		virtual void set_icon(cursor_icon& icon) override;
		virtual void set_raw_location(const point& location) override;
		virtual point get_hotspot_location() const override;

		void remove_icon();
		void on_icon_device_surface_changed(d3d9_cursor_icon& icon);

	private:
		void sync_device_to_active_icon();
	};

}