#include "prim2d_embedded_code.h"

namespace solar {

	const char* PRIM2D_EMBEDDED_CODE = R"(
		texture diffuse_texture;
		sampler diffuse_sampler = sampler_state{
			Texture = <diffuse_texture>;
			MipFilter = LINEAR;
			MinFilter = LINEAR;
			MagFilter = LINEAR;
			AddressU = CLAMP;
			AddressV = CLAMP;
			SRGBTexture = FALSE;
		};
		
		struct vs_input {
			float4 position : POSITION;
			float4 color : COLOR0;
			float2 tex_coord : TEXCOORD0;
		};
		
		struct ps_input {
			float4 position : POSITION;
			float4 color : COLOR0;
			float2 tex_coord : TEXCOORD0;
		};
		
		ps_input vs_render(vs_input input) {
			ps_input output;
			output.position = input.position;
			output.color = input.color;
			output.tex_coord = input.tex_coord;
			return output;
		}
		
		float4 ps_render(ps_input input) : COLOR{
			return tex2D(diffuse_sampler, input.tex_coord) * input.color;
		}
		
		technique render{ pass pass_0{
			VertexShader = compile vs_2_0 vs_render();
			PixelShader = compile ps_2_0 ps_render();
		}})";

}