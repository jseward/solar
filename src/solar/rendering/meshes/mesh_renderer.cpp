#include "mesh_renderer.h"
#include "mesh.h"

namespace solar {

	void mesh_renderer::render_full_mesh(const mesh& mesh, const mat44& world_transform) {
		for (int i = 0; i < mesh.get_sub_mesh_count(); ++i) {
			begin_rendering_sub_mesh(mesh, i, world_transform);
			render_sub_mesh(mesh, i);
			end_rendering_sub_mesh(mesh, i);
		}
	}

}