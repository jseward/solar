#pragma once

#include "solar/archiving/archivable.h"
#include "solar/math/vec3.h"
#include "solar/containers/fixed_vector.h"
#include "solar/rendering/textures/uv.h"
#include "mesh_vertex_bone_weight.h"

namespace solar {

	class mesh_vertex : archivable {
	public:
		enum { MAX_BONE_WEIGHT_COUNT = 4 };

	public:
		vec3 _position;
		vec3 _normal;
		vec3 _tangent;
		float _binormal_sign;
		uv _uv;
		fixed_vector<mesh_vertex_bone_weight, MAX_BONE_WEIGHT_COUNT> _bone_weights;

	public:
		mesh_vertex();

		virtual void read_from_archive(archive_reader& reader) override;
		virtual void write_to_archive(archive_writer& writer) const override;
	};

}