#include "solar_platforms/opengl/opengl_context.cpp"
#include "solar_platforms/opengl/opengl_mesh.cpp"
#include "solar_platforms/opengl/opengl_mesh_factory.cpp"
#include "solar_platforms/opengl/opengl_prim2d.cpp"
#include "solar_platforms/opengl/opengl_prim2d_lines.cpp"
#include "solar_platforms/opengl/opengl_prim2d_vertex.cpp"
#include "solar_platforms/opengl/opengl_shader_program.cpp"
#include "solar_platforms/opengl/opengl_shader_program_def.cpp"
#include "solar_platforms/opengl/opengl_shader_program_factory.cpp"
#include "solar_platforms/opengl/opengl_texture.cpp"
#include "solar_platforms/opengl/opengl_texture_factory.cpp"
#include "solar_platforms/opengl/opengl_render_device.cpp"
#include "solar_platforms/opengl/opengl_render_state_block.cpp"
#include "solar_platforms/opengl/opengl_user_settings.cpp"
#include "solar_platforms/opengl/opengl_window_type.cpp"
#include "solar_platforms/opengl/opengl_font_renderer_shader_program_provider.cpp"

#ifdef _WIN32
#include "solar_platforms/opengl/platforms/win32_opengl_context.cpp"
#else
#error("unknown platform")
#endif
