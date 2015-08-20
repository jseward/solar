#pragma once

#include "d3d9_headers.h"
#include "d3d9_format_helpers.h"
#include "solar/utility/verify.h"

namespace solar {

	template<typename T> void fill_texture_surface(IDirect3DTexture9* texture, int level, T value) {
		IF_VERIFY(texture != nullptr) {
			
			D3DSURFACE_DESC desc;
			D3D9_VERIFY(texture->GetLevelDesc(level, &desc));
			unsigned int format_size_in_bytes = get_format_bit_count(desc.Format) / 8;
			ASSERT(format_size_in_bytes == sizeof(T));

			D3DLOCKED_RECT rect;
			D3D9_VERIFY(texture->LockRect(level, &rect, NULL, 0));

			for (unsigned int y = 0; y < desc.Height; ++y)
			{
				unsigned char* addr = reinterpret_cast<unsigned char*>(rect.pBits) + (y * rect.Pitch);
				for (unsigned int x = 0; x < desc.Width; ++x)
				{
					*reinterpret_cast<T*>(addr) = value;
					addr += format_size_in_bytes;
				}
			}

			D3D9_VERIFY(texture->UnlockRect(level));
		}
	}

}

