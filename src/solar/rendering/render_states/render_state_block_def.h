#pragma once

#include "solar/utility/optional.h"
#include "render_state_enums.h"

namespace solar {

	class render_state_block_def {
	public:
		class blend {
		public:
			render_state_blend_type _src_rbg;
			render_state_blend_type _src_a;
			render_state_blend_type _dst_rbg;
			render_state_blend_type _dst_a;

		public:
			blend();
			blend(render_state_blend_type src, render_state_blend_type dst);
			blend(render_state_blend_type src_rgb, render_state_blend_type dst_rgb, render_state_blend_type src_a, render_state_blend_type dst_a);
		};

	public:
		render_state_color_write _color_write;
		render_state_depth_write _depth_write;
		render_state_compare_func _depth_compare_func;
		render_state_cull_mode _cull_mode;
		float _alpha_ref;
		bool _msaa_enabled;
		optional<blend> _blend;

	public:
		render_state_block_def();

		render_state_block_def& set_color_write(render_state_color_write write) {
			_color_write = write;
			return *this;
		}

		render_state_block_def& set_depth_write(render_state_depth_write write) {
			_depth_write = write;
			return *this;
		}

		render_state_block_def& set_depth_compare_func(render_state_compare_func func) {
			_depth_compare_func = func;
			return *this;
		}

		render_state_block_def& set_cull_mode(render_state_cull_mode cull_mode) {
			_cull_mode = cull_mode;
			return *this;
		}

		render_state_block_def& set_alpha_ref(float ref) {
			_alpha_ref = ref;
			return *this;
		}

		render_state_block_def& set_msaa_enabled(bool enabled) {
			_msaa_enabled = enabled;
			return *this;
		}

		render_state_block_def& set_blend(render_state_blend_type src, render_state_blend_type dst) {
			_blend = blend(src, dst);
			return *this;
		}

		render_state_block_def& set_blend(render_state_blend_type src_rgb, render_state_blend_type dst_rgb, render_state_blend_type src_a, render_state_blend_type dst_a) {
			_blend = blend(src_rgb, dst_rgb, src_a, dst_a);
			return *this;
		}

	};

}