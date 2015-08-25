#include "d3d9_prim2d_vertex.h"

namespace solar {

	//D3DFVF_SPECULAR is to help with 32bit aligned. Maps to the D3DCOLOR align32.
	const DWORD d3d9_prim2d_vertex::FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1;

	void d3d9_prim2d_vertex::set(const vec2& pos, const color& color, const uv& uv) {
		//Have to fix up sx and sy so that they are pixel accurate in Direct3D.
		//See Directly Mapping Texels to Pixels in docs.
		_sx = pos._x - 0.5f;
		_sy = pos._y - 0.5f;
		_sz = 0.f;
		_rhw = 1.f;
		_color = color.to_argb32();
		_tu = uv.u();
		_tv = uv.v();
	}

}