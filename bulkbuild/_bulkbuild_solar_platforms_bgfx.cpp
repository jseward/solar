#include "solar_platforms/bgfx/bgfx_cursor.cpp"
#include "solar_platforms/bgfx/bgfx_cursor_icon.cpp"
#include "solar_platforms/bgfx/bgfx_cursor_icon_factory.cpp"
#include "solar_platforms/bgfx/bgfx_mesh.cpp"
#include "solar_platforms/bgfx/bgfx_mesh_factory.cpp"
#include "solar_platforms/bgfx/bgfx_prim2d.cpp"
#include "solar_platforms/bgfx/bgfx_prim2d_lines.cpp"
#include "solar_platforms/bgfx/bgfx_shader_program.cpp"
#include "solar_platforms/bgfx/bgfx_shader_program_def.cpp"
#include "solar_platforms/bgfx/bgfx_shader_program_factory.cpp"
#include "solar_platforms/bgfx/bgfx_texture.cpp"
#include "solar_platforms/bgfx/bgfx_texture_factory.cpp"
#include "solar_platforms/bgfx/bgfx_render_device.cpp"
#include "solar_platforms/bgfx/bgfx_render_state_block.cpp"
#include "solar_platforms/bgfx/bgfx_user_settings.cpp"
#include "solar_platforms/bgfx/bgfx_window_type.cpp"
#include "solar_platforms/bgfx/bgfx_callback.cpp"
#include "solar_platforms/bgfx/bgfx_font_renderer_shader_program_provider.cpp"

#include "solar_platforms/bgfx/shaders/prim2d_shaders_embedded_code.cpp"
#include "solar_platforms/bgfx/shaders/font_shaders_embedded_code.cpp"

#ifdef _WIN32
#include "solar_platforms/bgfx/platforms/win32_bgfx_context.cpp"
#else
#error("unknown platform")
#endif
