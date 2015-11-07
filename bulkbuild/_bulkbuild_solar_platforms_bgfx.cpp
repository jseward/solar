#include "solar_platforms/bgfx/bgfx_cursor.cpp"
#include "solar_platforms/bgfx/bgfx_cursor_icon.cpp"
#include "solar_platforms/bgfx/bgfx_cursor_icon_factory.cpp"
#include "solar_platforms/bgfx/bgfx_mesh.cpp"
#include "solar_platforms/bgfx/bgfx_mesh_factory.cpp"
#include "solar_platforms/bgfx/bgfx_prim2d.cpp"
#include "solar_platforms/bgfx/bgfx_prim2d_lines.cpp"
#include "solar_platforms/bgfx/bgfx_shader.cpp"
#include "solar_platforms/bgfx/bgfx_shader_factory.cpp"
#include "solar_platforms/bgfx/bgfx_texture.cpp"
#include "solar_platforms/bgfx/bgfx_texture_factory.cpp"

#ifdef _WIN32
#include "solar_platforms/bgfx/bgfx_win32_render_device.cpp"
#else
#error("unknown platform")
#endif
