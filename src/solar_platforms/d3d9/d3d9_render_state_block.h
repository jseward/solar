#pragma once

#include <stdint.h>
#include "solar/rendering/render_states/render_state_block.h"
#include "solar/rendering/render_states/render_state_block_def.h"
#include "solar/utility/optional.h"
#include "d3d9_headers.h"
#include "d3d9_render_state_defines.h"

namespace solar {

	class d3d9_render_state_block : public render_state_block {
	public:
		class apply_result {
		public:
			uint64_t _new_flags;
			optional<float> _new_alpha_ref;
		};

	private:
		const render_state_block_def _def;
		uint64_t _flags;

	public:
		d3d9_render_state_block(const render_state_block_def& def);

		apply_result apply(IDirect3DDevice9* device, uint64_t device_flags);

	private:
		static uint64_t blend_type_to_state_define(render_state_blend_type bt);

		static D3DCULL get_cull_mode(uint32_t cull);
		static D3DCMPFUNC get_compare_func(uint32_t func);
		static D3DBLENDOP get_blend_op(uint32_t op);
		static D3DBLEND get_blend(uint32_t b);
	};

}