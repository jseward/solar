#pragma once

namespace solar {

	class mesh {
	public:
		virtual void create_rendering_resources() = 0;
		virtual void release_rendering_resources() = 0;
		virtual int get_sub_mesh_count() const = 0;
	};

}