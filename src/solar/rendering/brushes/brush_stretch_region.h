#pragma once

namespace solar {

	enum class brush_stretch_region {
		invalid = -1,

		CENTER,
		TOP_LEFT,
		TOP,
		TOP_RIGHT,
		RIGHT,
		BOTTOM_RIGHT,
		BOTTOM,
		BOTTOM_LEFT,
		LEFT,

		count
	};

}