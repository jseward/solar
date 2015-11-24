#include "font_normal_embedded_code.h"

namespace solar {

	const char* FONT_NORMAL_EMBEDDED_CODE = R"(
		float font_scale;

		texture diffuse_texture;
		sampler diffuse_sampler = sampler_state{
			Texture = <diffuse_texture>;
			MipFilter = NONE;
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
			float4 final_color = input.color;
		
			float font_exterior_smoothness = 1.f / (128.f * font_scale);
			float font_interior_smoothness = font_exterior_smoothness / 4.f;
			float min_distance = 0.5f - font_exterior_smoothness;
			float max_distance = 0.5f + font_interior_smoothness;
			float distance = tex2D(diffuse_sampler, input.tex_coord).r;
			final_color.a *= smoothstep(min_distance, max_distance, distance);
			return final_color;
		}
		
		technique render {
			pass pass_0{
				VertexShader = compile vs_2_0 vs_render();
				PixelShader = compile ps_2_0 ps_render();
			}
		})";

}