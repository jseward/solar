#include "d3d9_render_device.h"

#include "solar/utility/assert.h"
#include "solar/containers/container_helpers.h"
#include "solar/utility/unused_parameter.h"
#include "d3d9_headers.h"
#include "d3d9_verify.h"

namespace solar {

	d3d9_render_device::d3d9_render_device(d3d9_context& context)
		: _context(context) {
	}

	d3d9_render_device::~d3d9_render_device() {
		ASSERT(_event_handlers.empty());
	}

	void d3d9_render_device::setup() {
		_context.add_device_event_handler(this);
	}

	void d3d9_render_device::teardown() {
		_context.remove_device_event_handler(this);
	}

	void d3d9_render_device::add_event_handler(render_device_event_handler* handler) {
		push_back_and_verify_not_found(_event_handlers, handler);
	}

	void d3d9_render_device::remove_event_handler(render_device_event_handler* handler) {
		find_and_erase(_event_handlers, handler);
	}

	void d3d9_render_device::attempt_render_scene(render_scene_functor& func) {
		_context.attempt_render_scene(func);
	}

	bool d3d9_render_device::begin_scene() {
		return SUCCEEDED(_context.get_device()->BeginScene());
	}

	void d3d9_render_device::end_scene() {
		D3D9_VERIFY(_context.get_device()->EndScene());
	}

	void d3d9_render_device::clear(const color& color) {
		D3D9_VERIFY(_context.get_device()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, color.to_argb32(), 1.f, 0));
	}

	void d3d9_render_device::toggle_virtual_fullscreen() {
		_context.toggle_virtual_fullscreen();
	}

	viewport d3d9_render_device::set_viewport(const viewport& new_viewport) {
		ASSERT(new_viewport._min_z >= 0.f);
		ASSERT(new_viewport._max_z <= 1.f);
		ASSERT(new_viewport._min_z <= new_viewport._max_z);
		ASSERT(new_viewport._x + new_viewport._width <= _context.get_backbuffer_width());
		ASSERT(new_viewport._y + new_viewport._height <= _context.get_backbuffer_height());

		viewport old_viewport;
		D3D9_VERIFY(_context.get_device()->GetViewport(reinterpret_cast<D3DVIEWPORT9*>(&old_viewport)));
		D3D9_VERIFY(_context.get_device()->SetViewport(reinterpret_cast<const D3DVIEWPORT9*>(&new_viewport)));
		return old_viewport;
	}

	size d3d9_render_device::get_backbuffer_size() const {
		return _context.get_backbuffer_size();
	}

	void d3d9_render_device::on_device_created(IDirect3DDevice9* device) {
		UNUSED_PARAMETER(device);
	}

	void d3d9_render_device::on_device_released(IDirect3DDevice9* device) {
		UNUSED_PARAMETER(device);
	}

	void d3d9_render_device::on_device_reset(IDirect3DDevice9* device) {
		UNUSED_PARAMETER(device);
		for (auto event_handler : _event_handlers) {
			event_handler->on_render_device_backbuffer_size_changed(_context.get_backbuffer_size());
		}
	}

	void d3d9_render_device::on_device_lost(IDirect3DDevice9* device) {
		UNUSED_PARAMETER(device);
	}

}