#pragma once

#include "solar/utility/type_convert.h"

namespace solar {

	class d3d9_mesh_vertex {
	public:
		float _px;
		float _py;
		float _pz;
		float _nx;
		float _ny;
		float _nz;
		float _u_0;
		float _v_0;
		float _tx;
		float _ty;
		float _tz;
		float _binormal_sign;
		unsigned char _bone_index_0;
		unsigned char _bone_index_1;
		unsigned char _bone_index_2;
		unsigned char _bone_index_3;
		float _bone_weight_0;
		float _bone_weight_1;
		float _bone_weight_2;
		float _bone_weight_3;

	public:
		d3d9_mesh_vertex& operator=(const mesh_vertex& rhs) {
			_px = rhs._position.x();
			_py = rhs._position.y();
			_pz = rhs._position.z();
			_nx = rhs._normal.x();
			_ny = rhs._normal.y();
			_nz = rhs._normal.z();
			_u_0 = rhs._uv.u();
			_v_0 = rhs._uv.v();
			_tx = rhs._tangent.x();
			_ty = rhs._tangent.y();
			_tz = rhs._tangent.z();
			_binormal_sign = rhs._binormal_sign;

			copy_bone_weight(rhs, 0, _bone_index_0, _bone_weight_0);
			copy_bone_weight(rhs, 1, _bone_index_1, _bone_weight_1);
			copy_bone_weight(rhs, 2, _bone_index_2, _bone_weight_2);
			copy_bone_weight(rhs, 3, _bone_index_3, _bone_weight_3);

			return *this;
		}

	private:
		static void copy_bone_weight(const mesh_vertex& src, unsigned int i, unsigned char& dst_bone_index, float& dst_bone_weight) {
			if (src._bone_weights.size() > i) {
				dst_bone_index = ushort_to_uchar(src._bone_weights[i]._bone_index);
				dst_bone_weight = src._bone_weights[i]._weight;
			}
			else {
				dst_bone_index = 0;
				dst_bone_weight = 0.f;
			}
		}
	};

}