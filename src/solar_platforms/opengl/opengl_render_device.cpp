#include "opengl_render_device.h"

#include "solar/utility/assert.h"
#include "solar/containers/container_helpers.h"
#include "solar/utility/unused_parameter.h"
#include "opengl_render_state_block.h"

namespace solar {

	opengl_render_device::opengl_render_device(opengl_context& context) 
		: _context(context) {

		_context.add_backbuffer_size_changed_callback([this](const size& new_backbuffer_size) {
			for (auto& eh : _event_handlers) {
				eh->on_render_device_backbuffer_size_changed(new_backbuffer_size);
			}
		});
	}

	opengl_render_device::~opengl_render_device() {
		ASSERT(_event_handlers.empty());
	}

	void opengl_render_device::add_event_handler(render_device_event_handler* handler) {
		push_back_and_verify_not_found(_event_handlers, handler);
	}

	void opengl_render_device::remove_event_handler(render_device_event_handler* handler) {
		find_and_erase(_event_handlers, handler);
	}

	void opengl_render_device::attempt_render_scene(render_scene_functor& func) {
		_context.attempt_render_scene(func);		
	}

	bool opengl_render_device::begin_scene() {
		ASSERT(false);//todo
		//auto bb_size = _context.get_backbuffer_size();
		return true;
	}

	void opengl_render_device::end_scene() {
		ASSERT(false);//todo
		//opengl::frame();
	}

	void opengl_render_device::clear(const color& color) {
		ASSERT(false);//todo
		UNUSED_PARAMETER(color);
		//opengl::setViewClear(
		//	_context.get_current_view_id(), 
		//	opengl_CLEAR_COLOR | opengl_CLEAR_DEPTH,
		//	color.to_rgba32(),
		//	1.0f,
		//	0);
	}

	void opengl_render_device::toggle_virtual_fullscreen() {
		_context.toggle_virtual_fullscreen();
	}

	viewport opengl_render_device::set_viewport(const viewport& new_viewport) {
		//todo - needs a viewport id? - note: begin_scene calls opengl::setViewRect
		UNUSED_PARAMETER(new_viewport);
		ASSERT(false);
		return viewport();
	}

	size opengl_render_device::get_backbuffer_size() const {
		return _context.get_backbuffer_size();
	}

	render_state_block* opengl_render_device::create_render_state_block(const render_state_block_def& def) {
		return _context.create_render_state_block(def);
	}

	void opengl_render_device::release_render_state_block(render_state_block* block) {
		_context.release_render_state_block(block);
	}

}
