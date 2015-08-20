#include "d3d9_string_convert.h"

namespace solar {

	const char* D3DFORMAT_to_string(D3DFORMAT format) {
		switch (format) {
			case D3DFMT_A16B16G16R16: return "A16B16G16R16";
			case D3DFMT_A2B10G10R10: return "A2B10G10R10";
			case D3DFMT_A2R10G10B10: return "A2R10G10B10";
			case D3DFMT_R8G8B8: return "R8G8B8";
			case D3DFMT_A8R8G8B8: return "A8R8G8B8";
			case D3DFMT_X8R8G8B8: return "X8R8G8B8";
			case D3DFMT_A8B8G8R8: return "A8B8G8R8";
			case D3DFMT_R5G6B5: return "R5G6B5";
			case D3DFMT_X1R5G5B5: return "X1R5G5B5";
			case D3DFMT_A1R5G5B5: return "A1R5G5B5";
			case D3DFMT_A4R4G4B4: return "A4R4G4B4";
			case D3DFMT_X4R4G4B4: return "X4R4G4B4";
			case D3DFMT_R3G3B2: return "R3G3B2";
			case D3DFMT_A8R3G3B2: return "A8R3G3B2";
			case D3DFMT_D16_LOCKABLE: return "D16_LOCKABLE";
			case D3DFMT_D16: return "D16";
			case D3DFMT_D32F_LOCKABLE: return "D32F_LOCKABLE";
			case D3DFMT_D32: return "D32";
			case D3DFMT_R32F: return "R32F";
			case D3DFMT_R16F: return "R16F";
			case D3DFMT_L16: return "L16";
			case D3DFMT_D24X8: return "D24X8";
			case D3DFMT_D15S1: return "D15S1";
			case D3DFMT_D24X4S4: return "D24X4S4";
			case D3DFMT_D24S8: return "D24S8";
			case D3DFMT_D24FS8: return "D24FS8";
			case D3DFMT_DXT5: return "DXT5";
			default: return "???";
		}
	}

	const char* D3DDEVTYPE_to_string(D3DDEVTYPE type) {
		switch (type) {
			case D3DDEVTYPE_HAL: return "HAL";
			case D3DDEVTYPE_NULLREF: return "NULLREF";
			case D3DDEVTYPE_REF: return "REF";
			case D3DDEVTYPE_SW: return "SW";
			default: return "???";
		}
	}

	const char* ID3DXBuffer_to_string(ID3DXBuffer* buffer) {
		if (buffer != nullptr) {
			return static_cast<const char*>(buffer->GetBufferPointer());
		}
		return "";
	}

}