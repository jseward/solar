#pragma once

#include <functional>
#include "solar/math/size.h"
#include "solar/rendering/render_scene_functor.h"

namespace solar {

	//bgfx_context is the base class for hiding any platform specific details of using bfgx (ex. HWND resizing, initialization, etc.)

	class bgfx_context {
	public:
		using backbuffer_size_changed_callback = std::function<void(const size&)>;

	public:
		virtual void add_backbuffer_size_changed_callback(backbuffer_size_changed_callback callback) = 0;
		virtual void toggle_virtual_fullscreen() = 0;
		virtual size get_backbuffer_size() const = 0;
		virtual void attempt_render_scene(render_scene_functor& func) = 0;
	};

}