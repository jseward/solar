#include "render_state_block_def.h"

namespace solar {

	render_state_block_def::blend::blend() 
		: _src_rbg(render_state_blend_type::ZERO)
		, _dst_rbg(render_state_blend_type::ZERO) 
		, _src_a(render_state_blend_type::ZERO)
		, _dst_a(render_state_blend_type::ZERO){
	}

	render_state_block_def::blend::blend(render_state_blend_type src, render_state_blend_type dst)
		: _src_rbg(src)
		, _dst_rbg(dst)
		, _src_a(src)
		, _dst_a(dst) {
	}

	render_state_block_def::blend::blend(render_state_blend_type src_rgb, render_state_blend_type dst_rgb, render_state_blend_type src_a, render_state_blend_type dst_a)
		: _src_rbg(src_rgb)
		, _dst_rbg(dst_rgb)
		, _src_a(src_a)
		, _dst_a(dst_a) {
	}

	render_state_block_def::render_state_block_def()
		: _color_write(render_state_color_write::ENABLED_RGBA)
		, _depth_write(render_state_depth_write::DISABLED)
		, _depth_compare_func(render_state_compare_func::NONE)
		, _cull_mode(render_state_cull_mode::CCW)
		, _alpha_ref(0.f) 
		, _msaa_enabled(false)
		, _blend() {
	}

}
