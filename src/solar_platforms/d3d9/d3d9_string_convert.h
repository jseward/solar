#pragma once

#include "d3d9_headers.h"

namespace solar {

	const char* D3DFORMAT_to_string(D3DFORMAT format);
	const char* D3DDEVTYPE_to_string(D3DDEVTYPE type);
	const char* ID3DXBuffer_to_string(ID3DXBuffer* buffer);

}