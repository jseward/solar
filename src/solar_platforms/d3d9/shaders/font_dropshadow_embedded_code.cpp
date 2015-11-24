#include "font_dropshadow_embedded_code.h"

namespace solar {

	const char* FONT_DROPSHADOW_EMBEDDED_CODE = R"(
		float texture_pixel_width;
		float texture_pixel_height;
		float2 dropshadow_offset;
		float dropshadow_min_distance;
		float dropshadow_max_distance;

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
			float2 offset = float2(texture_pixel_width * -dropshadow_offset.x, texture_pixel_height * -dropshadow_offset.y);
			float distance = tex2D(diffuse_sampler, input.tex_coord + offset).r;
			final_color.a *= smoothstep(dropshadow_min_distance, dropshadow_max_distance, distance);
			return final_color;
		}
		
		technique render {
			pass pass_0{
				VertexShader = compile vs_2_0 vs_render();
				PixelShader = compile ps_2_0 ps_render();
			}
		})";

}