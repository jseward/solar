#include "d3d9_cursor.h"

#include "d3d9_context.h"
#include "d3d9_verify.h"
#include "solar/utility/assert.h"

namespace solar {

	d3d9_cursor::d3d9_cursor(d3d9_context& context)
		: _context(context)
		, _active_icon(nullptr)
		, _raw_location() {
	}

	d3d9_cursor::~d3d9_cursor() {
		ASSERT(_active_icon == nullptr);
	}

	void d3d9_cursor::set_icon(cursor_icon& icon) {
		_active_icon = static_cast<d3d9_cursor_icon*>(&icon);
		sync_device_to_active_icon();
	}

	void d3d9_cursor::set_raw_location(const point& location) {
		_raw_location = location;
	}

	point d3d9_cursor::get_hotspot_location() const {
		point hotspot_location = _raw_location;
		if (_active_icon != nullptr) {
			hotspot_location += _active_icon->get_hotspot_offset();
		}
		return hotspot_location;
	}

	void d3d9_cursor::sync_device_to_active_icon() {
		auto device = _context.get_device();
		if (device != nullptr) {
			if (_active_icon != nullptr && _active_icon->get_device_surface() != nullptr) {
				D3D9_VERIFY(device->SetCursorProperties(
					_active_icon->get_hotspot_offset()._x,
					_active_icon->get_hotspot_offset()._y,
					_active_icon->get_device_surface()));
			}
		}
	}

	void d3d9_cursor::remove_icon() {
		_active_icon = nullptr;
	}

	void d3d9_cursor::on_icon_device_surface_changed(d3d9_cursor_icon& icon) {
		if (_active_icon == &icon) {
			sync_device_to_active_icon();
		}
	}

}