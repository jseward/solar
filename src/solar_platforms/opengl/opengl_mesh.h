#pragma once

#include "solar/rendering/meshes/mesh.h"
#include "solar/resources/resource_address.h"

namespace solar {

	class opengl_mesh : public mesh {
	private:
		resource_address _resource_address;

	public:
		opengl_mesh(const resource_address& resource_address);
		virtual ~opengl_mesh();

		const resource_address& get_resource_address() const;

		virtual void create_rendering_resources() override;
		virtual void release_rendering_resources() override;
		virtual int get_sub_mesh_count() const override;
	};

}