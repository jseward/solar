#pragma once

#include <functional>
#include "solar/math/size.h"
#include "solar/rendering/render_scene_functor.h"
#include "solar/rendering/render_states/render_state_block.h"
#include "solar/rendering/render_states/render_state_block_def.h"

namespace solar {

	//opengl_context is the base class for hiding any platform specific details of using bfgx (ex. HWND resizing, initialization, etc.)

	class opengl_context {
	public:
		using backbuffer_size_changed_callback = std::function<void(const size&)>;

	public:
		virtual void add_backbuffer_size_changed_callback(backbuffer_size_changed_callback callback) = 0;
		virtual void toggle_virtual_fullscreen() = 0;
		virtual size get_backbuffer_size() const = 0;
		virtual void attempt_render_scene(render_scene_functor& func) = 0;

		render_state_block* create_render_state_block(const render_state_block_def& def);
		void release_render_state_block(render_state_block* block);
	};

}
