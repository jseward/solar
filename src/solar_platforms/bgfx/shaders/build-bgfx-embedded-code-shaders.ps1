#expect shaderc.exe to live in .\solar_tools side-by-side .\solar
$shaderc = Resolve-Path "..\..\..\..\..\solar_tools\shaderc\shaderc.exe"

$shaders = @(
	@{ "src_name" = "prim2d.vsc"; "type" = "v"; "platform" = "windows"; "profile" = "vs_3_0"; "dst_constant_name" = "PRIM2D_VS_D3D9"; "dst_file_name" = "_prim2d_vs_d3d9.cpp" }
	@{ "src_name" = "prim2d.fsc"; "type" = "f"; "platform" = "windows"; "profile" = "ps_3_0"; "dst_constant_name" = "PRIM2D_FS_D3D9"; "dst_file_name" = "_prim2d_fs_d3d9.cpp" }
	@{ "src_name" = "font_normal.vsc"; "type" = "v"; "platform" = "windows"; "profile" = "vs_3_0"; "dst_constant_name" = "FONT_NORMAL_VS_D3D9"; "dst_file_name" = "_font_normal_vs_d3d9.cpp" }
	@{ "src_name" = "font_normal.fsc"; "type" = "f"; "platform" = "windows"; "profile" = "ps_3_0"; "dst_constant_name" = "FONT_NORMAL_FS_D3D9"; "dst_file_name" = "_font_normal_fs_d3d9.cpp" }
	@{ "src_name" = "font_dropshadow.vsc"; "type" = "v"; "platform" = "windows"; "profile" = "vs_3_0"; "dst_constant_name" = "FONT_DROPSHADOW_VS_D3D9"; "dst_file_name" = "_font_dropshadow_vs_d3d9.cpp" }
	@{ "src_name" = "font_dropshadow.fsc"; "type" = "f"; "platform" = "windows"; "profile" = "ps_3_0"; "dst_constant_name" = "FONT_DROPSHADOW_FS_D3D9"; "dst_file_name" = "_font_dropshadow_fs_d3d9.cpp" })
	
foreach ($shader in $shaders) {
	"$($shader.src_name) -> $($shader.dst_file_name)"
	. $shaderc -f $shader.src_name -o $shader.dst_constant_name --type $shader.type --platform $shader.platform --profile $shader.profile --bin2c
	cp $shader.dst_constant_name $shader.dst_file_name
	del $shader.dst_constant_name
}
