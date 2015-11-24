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
		uint8_t _bone_index_0;
		uint8_t _bone_index_1;
		uint8_t _bone_index_2;
		uint8_t _bone_index_3;
		float _bone_weight_0;
		float _bone_weight_1;
		float _bone_weight_2;
		float _bone_weight_3;

	public:
		d3d9_mesh_vertex& operator=(const mesh_vertex& rhs) {
			_px = rhs._position._x;
			_py = rhs._position._y;
			_pz = rhs._position._z;
			_nx = rhs._normal._x;
			_ny = rhs._normal._y;
			_nz = rhs._normal._z;
			_u_0 = rhs._uv._u;
			_v_0 = rhs._uv._v;
			_tx = rhs._tangent._x;
			_ty = rhs._tangent._y;
			_tz = rhs._tangent._z;
			_binormal_sign = rhs._binormal_sign;

			copy_bone_weight(rhs, 0, _bone_index_0, _bone_weight_0);
			copy_bone_weight(rhs, 1, _bone_index_1, _bone_weight_1);
			copy_bone_weight(rhs, 2, _bone_index_2, _bone_weight_2);
			copy_bone_weight(rhs, 3, _bone_index_3, _bone_weight_3);

			return *this;
		}

	private:
		static void copy_bone_weight(const mesh_vertex& src, unsigned int i, uint8_t& dst_bone_index, float& dst_bone_weight) {
			if (src._bone_weights.size() > i) {
				dst_bone_index = src._bone_weights[i]._bone_index;
				dst_bone_weight = src._bone_weights[i]._weight;
			}
			else {
				dst_bone_index = 0;
				dst_bone_weight = 0.f;
			}
		}
	};

}