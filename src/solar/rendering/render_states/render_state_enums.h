#pragma once

namespace solar {

	enum class render_state_color_write {
		DISABLED,
		ENABLED_RGB_ONLY,
		ENABLED_RGBA
	};

	enum class render_state_depth_write {
		DISABLED,
		ENABLED
	};

	enum class render_state_cull_mode {
		NONE,
		CW,
		CCW
	};

	enum class render_state_compare_func {
		NONE,
		LESS,
		LESS_EQUAL,
		EQUAL,
		GREATER_EQUAL,
		GREATER,
		NOT_EQUAL,
		NEVER,
		ALWAYS
	};

	enum class render_state_blend_type {
		ZERO,
		ONE,
		SRC_COLOR,
		INV_SRC_COLOR,
		SRC_ALPHA,
		INV_SRC_ALPHA,
		DST_ALPHA,
		INV_DST_ALPHA,
		DST_COLOR,
		INV_DST_COLOR
	};

	enum class render_state_blend_equation {
		ADD,
		SUB,
		REVSUB,
		MIN,
		MAX
	};

}
