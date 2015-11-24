#include "opengl_context.h"

#include "opengl_render_state_block.h"

namespace solar {

	render_state_block* opengl_context::create_render_state_block(const render_state_block_def& def) {
		return new opengl_render_state_block(def);
	}

	void opengl_context::release_render_state_block(render_state_block* block) {
		delete block;
	}
}