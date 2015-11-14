#pragma once

#include "solar/rendering/render_states/render_state_block.h"
#include "solar/rendering/render_states/render_state_block_def.h"

namespace solar {

	class bgfx_render_state_block : public render_state_block {
	private:
		uint64_t _bgfx_state_flags;

	public:
		bgfx_render_state_block(const render_state_block_def& def);
		virtual ~bgfx_render_state_block();
		virtual void commit() override;
	};

}