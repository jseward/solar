#pragma once

#include <memory>
#include <functional>
#include "render_state_block.h"
#include "../render_device.h"

namespace solar {

	typedef std::unique_ptr<render_state_block, std::function<void(render_state_block*)>> render_state_block_ptr;

	inline render_state_block_ptr make_render_state_block_ptr(render_device& render_device, const render_state_block_def& def) {
		auto del = [&render_device](render_state_block* block) { render_device.release_render_state_block(block); };
		return render_state_block_ptr(render_device.create_render_state_block(def), del);
	}

}