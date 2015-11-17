#pragma once

#include <bgfx/bgfx.h>
#include "../bgfx_shader_embedded_code.h"

namespace solar {

	extern bgfx_shader_embedded_code get_prim2d_vs_embedded_code(bgfx::RendererType::Enum renderer_type);
	extern bgfx_shader_embedded_code get_prim2d_fs_embedded_code(bgfx::RendererType::Enum renderer_type);

}