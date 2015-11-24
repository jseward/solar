#include "bgfx_context.h"

#include "bgfx_render_state_block.h"

namespace solar {

	render_state_block* bgfx_context::create_render_state_block(const render_state_block_def& def) {
		return new bgfx_render_state_block(def);
	}

	void bgfx_context::release_render_state_block(render_state_block* block) {
		delete block;
	}
}