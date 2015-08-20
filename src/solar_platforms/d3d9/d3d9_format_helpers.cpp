#include "d3d9_format_helpers.h"
#include "solar/utility/assert.h"

namespace solar {

	int get_format_bit_count(D3DFORMAT format) {
		switch (format) {
			case D3DFMT_DXT1:
				return 4;

			case D3DFMT_R3G3B2:
			case D3DFMT_A8:
			case D3DFMT_P8:
			case D3DFMT_L8:
			case D3DFMT_A4L4:
			case D3DFMT_DXT2:
			case D3DFMT_DXT3:
			case D3DFMT_DXT4:
			case D3DFMT_DXT5:
				return 8;

			case D3DFMT_X4R4G4B4:
			case D3DFMT_A4R4G4B4:
			case D3DFMT_A1R5G5B5:
			case D3DFMT_X1R5G5B5:
			case D3DFMT_R5G6B5:
			case D3DFMT_A8R3G3B2:
			case D3DFMT_A8P8:
			case D3DFMT_A8L8:
			case D3DFMT_V8U8:
			case D3DFMT_L6V5U5:
			case D3DFMT_D16_LOCKABLE:
			case D3DFMT_D15S1:
			case D3DFMT_D16:
			case D3DFMT_L16:
			case D3DFMT_INDEX16:
			case D3DFMT_CxV8U8:
			case D3DFMT_G8R8_G8B8:
			case D3DFMT_R8G8_B8G8:
			case D3DFMT_R16F:
				return 16;

			case D3DFMT_R8G8B8:
				return 24;

			case D3DFMT_A2W10V10U10:
			case D3DFMT_A2B10G10R10:
			case D3DFMT_A2R10G10B10:
			case D3DFMT_X8R8G8B8:
			case D3DFMT_A8R8G8B8:
			case D3DFMT_X8L8V8U8:
			case D3DFMT_Q8W8V8U8:
			case D3DFMT_V16U16:
			case D3DFMT_UYVY:
			case D3DFMT_YUY2:
			case D3DFMT_G16R16:
			case D3DFMT_D32:
			case D3DFMT_D24S8:
			case D3DFMT_D24X8:
			case D3DFMT_D24X4S4:
			case D3DFMT_D24FS8:
			case D3DFMT_D32F_LOCKABLE:
			case D3DFMT_INDEX32:
			case D3DFMT_MULTI2_ARGB8:
			case D3DFMT_G16R16F:
			case D3DFMT_R32F:
				return 32;

			case D3DFMT_A16B16G16R16:
			case D3DFMT_Q16W16V16U16:
			case D3DFMT_A16B16G16R16F:
			case D3DFMT_G32R32F:
				return 64;

			case D3DFMT_A32B32G32R32F:
				return 128;

			default:
				ASSERT(false);
				return 0;
		}
	}

	int get_color_channel_bit_count(D3DFORMAT format) {
		switch (format) {
			case D3DFMT_A16B16G16R16:
				return 16;

			case D3DFMT_A2B10G10R10:
			case D3DFMT_A2R10G10B10:
				return 10;

			case D3DFMT_R8G8B8:
			case D3DFMT_A8R8G8B8:
			case D3DFMT_X8R8G8B8:
			case D3DFMT_A8B8G8R8:
				return 8;

			case D3DFMT_R5G6B5:
			case D3DFMT_X1R5G5B5:
			case D3DFMT_A1R5G5B5:
				return 5;

			case D3DFMT_A4R4G4B4:
			case D3DFMT_X4R4G4B4:
				return 4;

			case D3DFMT_R3G3B2:
			case D3DFMT_A8R3G3B2:
				return 2;

			default:
				ASSERT(false);
				return 0;
		}
	}

	int get_depth_format_bit_count(D3DFORMAT format) {
		switch (format) {
			case D3DFMT_D32F_LOCKABLE:
			case D3DFMT_D32:
				return 32;

			case D3DFMT_D24X8:
			case D3DFMT_D24S8:
			case D3DFMT_D24X4S4:
			case D3DFMT_D24FS8:
				return 24;

			case D3DFMT_D16_LOCKABLE:
			case D3DFMT_D16:
				return 16;

			case D3DFMT_D15S1:
				return 15;

			default:
				ASSERT(false);
				return 0;
		}
	}

	int get_stencil_format_bit_count(D3DFORMAT format) {
		switch (format) {
			case D3DFMT_D16_LOCKABLE:
			case D3DFMT_D16:
			case D3DFMT_D32F_LOCKABLE:
			case D3DFMT_D32:
			case D3DFMT_D24X8:
				return 0;

			case D3DFMT_D15S1:
				return 1;

			case D3DFMT_D24X4S4:
				return 4;

			case D3DFMT_D24S8:
			case D3DFMT_D24FS8:
				return 8;

			default:
				ASSERT(false);
				return 0;
		}
	}

}