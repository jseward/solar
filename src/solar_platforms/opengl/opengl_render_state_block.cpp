#include "opengl_render_state_block.h"

#include "solar/math/math_helpers.h"
#include "opengl_render_state_defines.h"

namespace solar {

	opengl_render_state_block::opengl_render_state_block(const render_state_block_def& def) 
		: _state(0) {

		switch (def._color_write) {
		case render_state_color_write::ENABLED_RGB_ONLY:
			_state |= OPENGL_STATE_RGB_WRITE; break;
		case render_state_color_write::ENABLED_RGBA:
			_state |= (OPENGL_STATE_RGB_WRITE | OPENGL_STATE_ALPHA_WRITE); break;
		default: break;
		}

		if (def._depth_write == render_state_depth_write::ENABLED) {
			_state |= OPENGL_STATE_DEPTH_WRITE;
		}

		switch (def._cull_mode) {
		case render_state_cull_mode::CW:
			_state |= OPENGL_STATE_CULL_CW; break;
		case render_state_cull_mode::CCW:
			_state |= OPENGL_STATE_CULL_CCW; break;
		default: break;
		}

		switch (def._depth_compare_func) {
		case render_state_compare_func::LESS:
			_state |= OPENGL_STATE_DEPTH_TEST_LESS; break;
		case render_state_compare_func::LESS_EQUAL:
			_state |= OPENGL_STATE_DEPTH_TEST_LEQUAL; break;
		case render_state_compare_func::EQUAL:
			_state |= OPENGL_STATE_DEPTH_TEST_EQUAL; break;
		case render_state_compare_func::GREATER_EQUAL:
			_state |= OPENGL_STATE_DEPTH_TEST_GEQUAL; break;
		case render_state_compare_func::GREATER:
			_state |= OPENGL_STATE_DEPTH_TEST_GREATER; break;
		case render_state_compare_func::NOT_EQUAL:
			_state |= OPENGL_STATE_DEPTH_TEST_NOTEQUAL; break;
		case render_state_compare_func::NEVER:
			_state |= OPENGL_STATE_DEPTH_TEST_NEVER; break;
		case render_state_compare_func::ALWAYS:
			_state |= OPENGL_STATE_DEPTH_TEST_ALWAYS; break;
		default: break;
		}

		int alpha_ref_byte = static_cast<int>(def._alpha_ref * 255.f);
		ASSERT(is_within_inclusive(0, 255, alpha_ref_byte));
		_state |= OPENGL_STATE_ALPHA_REF(alpha_ref_byte);

		if (def._msaa_enabled) {
			_state |= OPENGL_STATE_MSAA;
		}

		if (def._blend.has_value()) {
			_state |= OPENGL_STATE_BLEND_FUNC_SEPARATE(
				blend_type_to_state_define(def._blend.get_value()._src_rbg),
				blend_type_to_state_define(def._blend.get_value()._dst_rbg),
				blend_type_to_state_define(def._blend.get_value()._src_a),
				blend_type_to_state_define(def._blend.get_value()._dst_a));
		}

	}

	opengl_render_state_block::~opengl_render_state_block() {
	}

	uint64_t opengl_render_state_block::blend_type_to_state_define(render_state_blend_type bt) {
		switch (bt) {
		case render_state_blend_type::ZERO: return OPENGL_STATE_BLEND_ZERO;
		case render_state_blend_type::ONE: return OPENGL_STATE_BLEND_ONE;
		case render_state_blend_type::SRC_COLOR: return OPENGL_STATE_BLEND_SRC_COLOR;
		case render_state_blend_type::INV_SRC_COLOR: return OPENGL_STATE_BLEND_INV_SRC_COLOR;
		case render_state_blend_type::SRC_ALPHA: return OPENGL_STATE_BLEND_SRC_ALPHA;
		case render_state_blend_type::INV_SRC_ALPHA: return OPENGL_STATE_BLEND_INV_SRC_ALPHA;
		case render_state_blend_type::DST_ALPHA: return OPENGL_STATE_BLEND_DST_ALPHA;
		case render_state_blend_type::INV_DST_ALPHA: return OPENGL_STATE_BLEND_INV_DST_ALPHA;
		case render_state_blend_type::DST_COLOR: return OPENGL_STATE_BLEND_DST_COLOR;
		case render_state_blend_type::INV_DST_COLOR: return OPENGL_STATE_BLEND_INV_DST_COLOR;
		default: ASSERT(false);
		}
		return 0;
	}

	void opengl_render_state_block::commit() {
		//todo
		ASSERT(false);
	}

}
