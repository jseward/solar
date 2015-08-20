#pragma once

#include "d3d9_headers.h"

namespace solar {

	int get_format_bit_count(D3DFORMAT format);
	int get_color_channel_bit_count(D3DFORMAT format);
	int get_depth_format_bit_count(D3DFORMAT format);
	int get_stencil_format_bit_count(D3DFORMAT format);

}