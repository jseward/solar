#include "prim2d_shaders_embedded_code.h"

#include "solar/utility/assert.h"

#include "_prim2d_vs_d3d9.cpp"
#include "_prim2d_fs_d3d9.cpp"

namespace solar {

	bgfx_shader_embedded_code get_prim2d_vs_embedded_code(bgfx::RendererType::Enum renderer_type) {
		switch (renderer_type) {
			case bgfx::RendererType::Direct3D9: return make_bgfx_shader_embedded_code(PRIM2D_VS_D3D9);
			default: ASSERT(false); break;
		}
		return bgfx_shader_embedded_code();
	}

	bgfx_shader_embedded_code get_prim2d_fs_embedded_code(bgfx::RendererType::Enum renderer_type) {
		switch (renderer_type) {
			case bgfx::RendererType::Direct3D9: return make_bgfx_shader_embedded_code(PRIM2D_FS_D3D9);
			default: ASSERT(false); break;
		}
		return bgfx_shader_embedded_code();
	}

}