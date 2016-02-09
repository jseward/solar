#pragma once

namespace solar {

	class texture_create_params {
	public:
		bool _has_mip_maps;

	public:
		texture_create_params()
			: _has_mip_maps(false) {
		}

		bool operator==(const texture_create_params& rhs) const {
			return
				_has_mip_maps == rhs._has_mip_maps;
		}

		bool operator!=(const texture_create_params& rhs) const {
			return !(*this == rhs);
		}

		texture_create_params& set_has_mip_maps(bool has_mip_maps) {
			_has_mip_maps = has_mip_maps;
			return *this;
		}
	};

}