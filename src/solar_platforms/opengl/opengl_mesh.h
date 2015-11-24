#pragma once

#include "solar/rendering/meshes/mesh.h"
#include "solar/rendering/meshes/mesh_texture_params.h"
#include "solar/resources/resource_address.h"

namespace solar {

	class opengl_mesh : public mesh {
	private:
		mesh_texture_params _texture_params;
		resource_address _resource_address;

	public:
		opengl_mesh(const mesh_texture_params& texture_params, const resource_address& resource_address);
		virtual ~opengl_mesh();

		const resource_address& get_resource_address() const;
		const mesh_texture_params& get_texture_params() const;

		virtual void create_rendering_resources() override;
		virtual void release_rendering_resources() override;
		virtual int get_sub_mesh_count() const override;
	};

}