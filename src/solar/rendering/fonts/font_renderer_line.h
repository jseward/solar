#pragma once

#include "solar/math/pointf.h"

namespace solar {

	class font_renderer_line {
	public:
		unsigned int _begin_text_index;
		unsigned int _end_text_index;
		pointf _begin_top_left;

	public:
		font_renderer_line() 
			: _begin_text_index(0)
			, _end_text_index(0)
			, _begin_top_left() {
		}

		font_renderer_line(int begin_text_index, int end_text_index, const pointf& begin_top_left)
			: _begin_text_index(begin_text_index)
			, _end_text_index(end_text_index)
			, _begin_top_left(begin_top_left) {
		}
	};

}