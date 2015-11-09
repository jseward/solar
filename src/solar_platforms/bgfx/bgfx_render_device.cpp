#include "bgfx_render_device.h"

#include <bgfx/bgfx.h>
#include "solar/utility/assert.h"
#include "solar/containers/container_helpers.h"
#include "solar/utility/unused_parameter.h"

namespace solar {

	bgfx_render_device::bgfx_render_device(bgfx_context& context) 
		: _context(context) {

		_context.add_backbuffer_size_changed_callback([this](const size& new_backbuffer_size) {
			for (auto& eh : _event_handlers) {
				eh->on_render_device_backbuffer_size_changed(new_backbuffer_size);
			}
		});
	}

	bgfx_render_device::~bgfx_render_device() {
		ASSERT(_event_handlers.empty());
	}

	void bgfx_render_device::add_event_handler(render_device_event_handler* handler) {
		push_back_and_verify_not_found(_event_handlers, handler);
	}

	void bgfx_render_device::remove_event_handler(render_device_event_handler* handler) {
		find_and_erase(_event_handlers, handler);
	}

	void bgfx_render_device::attempt_render_scene(render_scene_functor& func) {
		_context.attempt_render_scene(func);		
	}

	bool bgfx_render_device::begin_scene() {
		auto bb_size = _context.get_backbuffer_size();
		bgfx::setViewRect(0, 0, 0, static_cast<uint16_t>(bb_size._width), static_cast<uint16_t>(bb_size._height));

		// This dummy draw call is here to make sure that view 0 is cleared
		// if no other draw calls are submitted to view 0.
		bgfx::touch(0);
		return true;
	}

	void bgfx_render_device::end_scene() {
		bgfx::frame();
	}

	void bgfx_render_device::clear(const color& color) {
		bgfx::setViewClear(
			0, 
			BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
			color.to_rgba32(),
			1.0f,
			0);
	}

	void bgfx_render_device::toggle_virtual_fullscreen() {
		_context.toggle_virtual_fullscreen();
	}

	viewport bgfx_render_device::set_viewport(const viewport& new_viewport) {
		//todo
		UNUSED_PARAMETER(new_viewport);
		ASSERT(false);
		return viewport();
	}

	size bgfx_render_device::get_backbuffer_size() const {
		return _context.get_backbuffer_size();
	}

}
