#include "d3d9_render_state_block.h"

#include "solar/utility/assert.h"
#include "solar/math/math_helpers.h"
#include "d3d9_render_state_defines.h"
#include "d3d9_verify.h"

namespace solar {

	d3d9_render_state_block::d3d9_render_state_block(const render_state_block_def& def) 
		: _def(def)
		, _flags(0) {

		switch (def._color_write) {
			case render_state_color_write::ENABLED_RGB_ONLY:
				_flags |= D3D9_STATE_RGB_WRITE; break;
			case render_state_color_write::ENABLED_RGBA:
				_flags |= (D3D9_STATE_RGB_WRITE | D3D9_STATE_ALPHA_WRITE); break;
			default: break;
		}

		if (def._depth_write == render_state_depth_write::ENABLED) {
			_flags |= D3D9_STATE_DEPTH_WRITE;
		}

		switch (def._cull_mode) {
			case render_state_cull_mode::CW:
				_flags |= D3D9_STATE_CULL_CW; break;
			case render_state_cull_mode::CCW:
				_flags |= D3D9_STATE_CULL_CCW; break;
			default: break;
		}

		switch (def._depth_compare_func) {
			case render_state_compare_func::LESS:
				_flags |= D3D9_STATE_DEPTH_TEST_LESS; break;
			case render_state_compare_func::LESS_EQUAL:
				_flags |= D3D9_STATE_DEPTH_TEST_LEQUAL; break;
			case render_state_compare_func::EQUAL:
				_flags |= D3D9_STATE_DEPTH_TEST_EQUAL; break;
			case render_state_compare_func::GREATER_EQUAL:
				_flags |= D3D9_STATE_DEPTH_TEST_GEQUAL; break;
			case render_state_compare_func::GREATER:
				_flags |= D3D9_STATE_DEPTH_TEST_GREATER; break;
			case render_state_compare_func::NOT_EQUAL:
				_flags |= D3D9_STATE_DEPTH_TEST_NOTEQUAL; break;
			case render_state_compare_func::NEVER:
				_flags |= D3D9_STATE_DEPTH_TEST_NEVER; break;
			case render_state_compare_func::ALWAYS:
				_flags |= D3D9_STATE_DEPTH_TEST_ALWAYS; break;
			default: break;
		}

		int alpha_ref_byte = static_cast<int>(def._alpha_ref * 255.f);
		ASSERT(is_within_inclusive(0, 255, alpha_ref_byte));
		_flags |= D3D9_STATE_ALPHA_REF(alpha_ref_byte);

		if (def._msaa_enabled) {
			_flags |= D3D9_STATE_MSAA;
		}

		if (def._blend.has_value()) {
			_flags |= D3D9_STATE_BLEND_FUNC_SEPARATE(
				blend_type_to_state_define(def._blend.get_value()._src_rbg),
				blend_type_to_state_define(def._blend.get_value()._dst_rbg),
				blend_type_to_state_define(def._blend.get_value()._src_a),
				blend_type_to_state_define(def._blend.get_value()._dst_a));
		}
	}

	uint64_t d3d9_render_state_block::blend_type_to_state_define(render_state_blend_type bt) {
		switch (bt) {
			case render_state_blend_type::ZERO: return D3D9_STATE_BLEND_ZERO;
			case render_state_blend_type::ONE: return D3D9_STATE_BLEND_ONE;
			case render_state_blend_type::SRC_COLOR: return D3D9_STATE_BLEND_SRC_COLOR;
			case render_state_blend_type::INV_SRC_COLOR: return D3D9_STATE_BLEND_INV_SRC_COLOR;
			case render_state_blend_type::SRC_ALPHA: return D3D9_STATE_BLEND_SRC_ALPHA;
			case render_state_blend_type::INV_SRC_ALPHA: return D3D9_STATE_BLEND_INV_SRC_ALPHA;
			case render_state_blend_type::DST_ALPHA: return D3D9_STATE_BLEND_DST_ALPHA;
			case render_state_blend_type::INV_DST_ALPHA: return D3D9_STATE_BLEND_INV_DST_ALPHA;
			case render_state_blend_type::DST_COLOR: return D3D9_STATE_BLEND_DST_COLOR;
			case render_state_blend_type::INV_DST_COLOR: return D3D9_STATE_BLEND_INV_DST_COLOR;
			default: ASSERT(false);
		}
		return 0;
	}

	d3d9_render_state_block::apply_result d3d9_render_state_block::apply(IDirect3DDevice9* device, uint64_t device_flags) {
		apply_result result;
		result._new_flags = _flags;
		
		auto changed_flags = device_flags ^ _flags;
		if (changed_flags != 0) {

			if (D3D9_STATE_CULL_MASK & changed_flags) {
				uint32_t cull = (_flags & D3D9_STATE_CULL_MASK) >> D3D9_STATE_CULL_SHIFT;
				D3D9_VERIFY(device->SetRenderState(D3DRS_CULLMODE, get_cull_mode(cull)));
			}

			if (D3D9_STATE_DEPTH_WRITE & changed_flags) {
				D3D9_VERIFY(device->SetRenderState(D3DRS_ZWRITEENABLE, !!(D3D9_STATE_DEPTH_WRITE & _flags)));
			}

			if (D3D9_STATE_DEPTH_TEST_MASK & changed_flags) {
				uint32_t func = (_flags & D3D9_STATE_DEPTH_TEST_MASK) >> D3D9_STATE_DEPTH_TEST_SHIFT;
				D3D9_VERIFY(device->SetRenderState(D3DRS_ZENABLE, 0 != func));
				if (0 != func) {
					D3D9_VERIFY(device->SetRenderState(D3DRS_ZFUNC, get_compare_func(func)));
				}
			}

			if (D3D9_STATE_ALPHA_REF_MASK & changed_flags) {
				uint32_t ref = (_flags & D3D9_STATE_ALPHA_REF_MASK) >> D3D9_STATE_ALPHA_REF_SHIFT;
				result._new_alpha_ref = static_cast<float>(ref) / 255.f;
			}

			if (D3D9_STATE_MSAA & changed_flags) {
				D3D9_VERIFY(device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, (_flags & D3D9_STATE_MSAA) == D3D9_STATE_MSAA));
			}

			if ((D3D9_STATE_ALPHA_WRITE | D3D9_STATE_RGB_WRITE) & changed_flags) {
				uint32_t write_enable = (_flags & D3D9_STATE_ALPHA_WRITE) ? D3DCOLORWRITEENABLE_ALPHA : 0;
				write_enable |= (_flags & D3D9_STATE_RGB_WRITE) ? D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE : 0;
				D3D9_VERIFY(device->SetRenderState(D3DRS_COLORWRITEENABLE, write_enable));
			}

			if ((D3D9_STATE_BLEND_MASK | D3D9_STATE_BLEND_EQUATION_MASK) & changed_flags) {
				
				bool enabled = !!(D3D9_STATE_BLEND_MASK & _flags);
				D3D9_VERIFY(device->SetRenderState(D3DRS_ALPHABLENDENABLE, enabled));

				if (enabled) {
					const uint32_t blend = static_cast<uint32_t>((_flags & D3D9_STATE_BLEND_MASK) >> D3D9_STATE_BLEND_SHIFT);
					const uint32_t equation = static_cast<uint32_t>((_flags & D3D9_STATE_BLEND_EQUATION_MASK) >> D3D9_STATE_BLEND_EQUATION_SHIFT);

					const uint32_t src_rgb = (blend) & 0xf;
					const uint32_t dst_rgb = (blend >> 4) & 0xf;
					const uint32_t src_a = (blend >> 8) & 0xf;
					const uint32_t dst_a = (blend >> 12) & 0xf;
					const uint32_t equ_rgb = (equation) & 0x7;
					const uint32_t equ_a = (equation >> 3) & 0x7;

					D3D9_VERIFY(device->SetRenderState(D3DRS_SRCBLEND, get_blend(src_rgb)));
					D3D9_VERIFY(device->SetRenderState(D3DRS_DESTBLEND, get_blend(dst_rgb)));
					D3D9_VERIFY(device->SetRenderState(D3DRS_BLENDOP, get_blend_op(equ_rgb)));

					const bool separate = 
						(src_rgb != src_a) || 
						(dst_rgb != dst_a) || 
						(equ_rgb != equ_a);

					D3D9_VERIFY(device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, separate));
					if (separate) {
						D3D9_VERIFY(device->SetRenderState(D3DRS_SRCBLENDALPHA, get_blend(src_a)));
						D3D9_VERIFY(device->SetRenderState(D3DRS_DESTBLENDALPHA, get_blend(dst_a)));
						D3D9_VERIFY(device->SetRenderState(D3DRS_BLENDOPALPHA, get_blend_op(equ_a)));
					}
				}
			}

		}

		return result;
	}

	D3DCULL d3d9_render_state_block::get_cull_mode(uint32_t cull) {
		switch (cull) {
			case 0: return D3DCULL_NONE;
			case 1: return D3DCULL_CW;
			case 2: return D3DCULL_CCW;
			default: ASSERT(false);
		}
		return D3DCULL_NONE;
	}

	D3DCMPFUNC d3d9_render_state_block::get_compare_func(uint32_t func) {
		switch (func) {
			case 0: return static_cast<D3DCMPFUNC>(0);
			case 1: return D3DCMP_LESS;
			case 2: return D3DCMP_LESSEQUAL;
			case 3: return D3DCMP_EQUAL;
			case 4: return D3DCMP_GREATEREQUAL;
			case 5: return D3DCMP_GREATER;
			case 6: return D3DCMP_NOTEQUAL;
			case 7: return D3DCMP_NEVER;
			case 8: return D3DCMP_ALWAYS;
			default: ASSERT(false);
		}
		return static_cast<D3DCMPFUNC>(0);
	}

	D3DBLENDOP d3d9_render_state_block::get_blend_op(uint32_t op) {
		switch (op) {
			case 0: return D3DBLENDOP_ADD;
			case 1: return D3DBLENDOP_SUBTRACT;
			case 2: return D3DBLENDOP_REVSUBTRACT;
			case 3: return D3DBLENDOP_MIN;
			case 4: return D3DBLENDOP_MAX;
			default: ASSERT(false);
		}
		return static_cast<D3DBLENDOP>(0);
	}

	D3DBLEND d3d9_render_state_block::get_blend(uint32_t b) {
		switch (b) {
			case 1: return D3DBLEND_ZERO;
			case 2: return D3DBLEND_ONE;
			case 3: return D3DBLEND_SRCCOLOR;
			case 4: return D3DBLEND_INVSRCCOLOR;
			case 5: return D3DBLEND_SRCALPHA;
			case 6: return D3DBLEND_INVSRCALPHA;
			case 7: return D3DBLEND_DESTALPHA;
			case 8: return D3DBLEND_INVDESTALPHA;
			case 9: return D3DBLEND_DESTCOLOR;
			case 10: return D3DBLEND_INVDESTCOLOR;
			default: ASSERT(false);
		}
		return static_cast<D3DBLEND>(0);
	}

}
