#pragma once

#include <unordered_map>
#include "solar/math/pointf.h"
#include "solar/math/sizef.h"
#include "solar/rendering/textures/simple_rect_uvs.h"

namespace solar {

	class font_glyph {
	public:
		pointf _top_left;
		sizef _size;
		pointf _offset;
		float _x_stride;
		unsigned int _page;
		int _channel;
		std::unordered_map<wchar_t, float> _kerning_offset_map;
		simple_rect_uvs _uvs;

	public:
		font_glyph() 
			: _x_stride(0)
			, _page(0)
			, _channel(0) {
		}

		float get_kerning_offset(wchar_t next_char) const {
			auto iter = _kerning_offset_map.find(next_char);
			if (iter != _kerning_offset_map.end()) {
				return iter->second;
			}
			return 0.f;
		}
	};

}