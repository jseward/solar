#include "font_shaders_embedded_code.h"

#include "solar/utility/assert.h"

#include "_font_normal_vs_d3d9.cpp"
#include "_font_normal_fs_d3d9.cpp"
#include "_font_dropshadow_vs_d3d9.cpp"
#include "_font_dropshadow_fs_d3d9.cpp"
#include "_font_normal_vs_d3d11.cpp"
#include "_font_normal_fs_d3d11.cpp"
#include "_font_dropshadow_vs_d3d11.cpp"
#include "_font_dropshadow_fs_d3d11.cpp"

namespace solar {

	bgfx_shader_embedded_code get_font_normal_vs_embedded_code(bgfx::RendererType::Enum renderer_type) {
		switch (renderer_type) {
			case bgfx::RendererType::Direct3D9: return make_bgfx_shader_embedded_code(FONT_NORMAL_VS_D3D9);
			case bgfx::RendererType::Direct3D11: return make_bgfx_shader_embedded_code(FONT_NORMAL_VS_D3D11);
			default: ASSERT(false); break;
		}
		return bgfx_shader_embedded_code();
	}

	bgfx_shader_embedded_code get_font_normal_fs_embedded_code(bgfx::RendererType::Enum renderer_type) {
		switch (renderer_type) {
			case bgfx::RendererType::Direct3D9: return make_bgfx_shader_embedded_code(FONT_NORMAL_FS_D3D9);
			case bgfx::RendererType::Direct3D11: return make_bgfx_shader_embedded_code(FONT_NORMAL_FS_D3D11);
			default: ASSERT(false); break;
		}
		return bgfx_shader_embedded_code();
	}

	bgfx_shader_embedded_code get_font_dropshadow_vs_embedded_code(bgfx::RendererType::Enum renderer_type) {
		switch (renderer_type) {
			case bgfx::RendererType::Direct3D9: return make_bgfx_shader_embedded_code(FONT_DROPSHADOW_VS_D3D9);
			case bgfx::RendererType::Direct3D11: return make_bgfx_shader_embedded_code(FONT_DROPSHADOW_VS_D3D11);
			default: ASSERT(false); break;
		}
		return bgfx_shader_embedded_code();
	}

	bgfx_shader_embedded_code get_font_dropshadow_fs_embedded_code(bgfx::RendererType::Enum renderer_type) {
		switch (renderer_type) {
			case bgfx::RendererType::Direct3D9: return make_bgfx_shader_embedded_code(FONT_DROPSHADOW_FS_D3D9);
			case bgfx::RendererType::Direct3D11: return make_bgfx_shader_embedded_code(FONT_DROPSHADOW_FS_D3D11);
			default: ASSERT(false); break;
		}
		return bgfx_shader_embedded_code();
	}

}