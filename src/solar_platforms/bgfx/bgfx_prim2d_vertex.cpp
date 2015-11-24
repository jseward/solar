#include "bgfx_prim2d_vertex.h"

namespace solar {

	void bgfx_prim2d_vertex::build_decl(bgfx::VertexDecl& decl, bgfx::RendererType::Enum renderer_type) {
		decl.begin(renderer_type);
		decl.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float);
		decl.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true);
		decl.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float);
		decl.end();
	}

}