#pragma once

#include "solar/rendering/render_states/render_state_block.h"
#include "solar/rendering/render_states/render_state_block_def.h"

namespace solar {

	class bgfx_render_state_block : public render_state_block {
	private:
		uint64_t _state;

	public:
		bgfx_render_state_block(const render_state_block_def& def);
		virtual ~bgfx_render_state_block();
		virtual void commit() override;

	private:
		static uint64_t blend_type_to_state_define(render_state_blend_type bt);
	};

}