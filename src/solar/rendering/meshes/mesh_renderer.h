#pragma once

namespace solar {

	class camera;
	class mat44;
	class mesh;
	class shader;

	class mesh_renderer {
	public:
		virtual void begin_rendering(const camera& camera, shader& shader) = 0;
		virtual void end_rendering() = 0;
		virtual void begin_rendering_sub_mesh(const mesh& mesh, int sub_mesh_index, const mat44& world_transform) = 0;
		virtual void end_rendering_sub_mesh(const mesh& mesh, int sub_mesh_index) = 0;
		virtual void render_sub_mesh(const mesh& mesh, int sub_mesh_index) = 0;

		void render_full_mesh(const mesh& mesh, const mat44& world_transform);
	};

}