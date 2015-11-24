#pragma once

#include <vector>
#include "solar/utility/property.h"
#include "d3d9_headers.h"
#include "d3d9_device_params.h"
#include "d3d9_user_settings.h"

namespace solar {

	struct d3d9_enumerator_setup_params {
		MAKE_PROPERTY_AS_COPY(d3d9_enumerator_setup_params, IDirect3D9*, IDirect3D9, nullptr);
		MAKE_PROPERTY(d3d9_enumerator_setup_params, UINT, min_display_mode_width, 640);
		MAKE_PROPERTY(d3d9_enumerator_setup_params, UINT, min_display_mode_height, 480);
		MAKE_PROPERTY(d3d9_enumerator_setup_params, bool, is_post_pixelshader_blending_required, true);
	};

	struct d3d9_enumerator_device_info {
		MAKE_PROPERTY(d3d9_enumerator_device_info, bool, is_valid, false);
		MAKE_PROPERTY(d3d9_enumerator_device_info, float, ranking, 0.f);
		MAKE_PROPERTY(d3d9_enumerator_device_info, D3DDEVTYPE, device_type, D3DDEVTYPE_HAL);
		MAKE_PROPERTY(d3d9_enumerator_device_info, UINT, adapter_ordinal, D3DADAPTER_DEFAULT);
		MAKE_PROPERTY(d3d9_enumerator_device_info, D3DFORMAT, adapter_format, D3DFMT_R8G8B8);
		MAKE_PROPERTY(d3d9_enumerator_device_info, DWORD, behavior_flags, D3DCREATE_SOFTWARE_VERTEXPROCESSING);
		MAKE_PROPERTY(d3d9_enumerator_device_info, D3DPRESENT_PARAMETERS, present_parameters, D3DPRESENT_PARAMETERS());
	};

	class d3d9_enumerator {

	private:
		struct device_combo {
			D3DFORMAT _adapter_format = D3DFMT_UNKNOWN;
			D3DFORMAT _backbuffer_format = D3DFMT_UNKNOWN;
			bool _is_windowed = false;
			std::vector<D3DFORMAT> _depth_stencil_formats;
			std::vector<UINT> _present_intervals;
		};

		struct device_info {
			D3DDEVTYPE _dev_type = D3DDEVTYPE_NULLREF;
			D3DCAPS9 _caps;
			std::vector<device_combo> _combos;
		};

		struct adapter_info {
			UINT _adapter_ordinal = 0;
			D3DADAPTER_IDENTIFIER9 _adapter_identifier;
			std::vector<D3DDISPLAYMODE> _display_modes;
			std::vector<device_info> _device_infos;
		};

	private:
		std::vector<adapter_info> _adapter_infos;

	public:
		d3d9_enumerator();

		void setup(const d3d9_enumerator_setup_params& params);
		d3d9_enumerator_device_info build_best_device_info_from_user_settings(IDirect3D9* idirect3d9, const d3d9_user_settings& user_settings) const;

	private:
		adapter_info build_adapter_info(const d3d9_enumerator_setup_params& params, UINT adapter_ordinal);

		std::vector<D3DFORMAT> get_potential_adapter_formats() const;
		std::vector<D3DFORMAT> get_potential_backbuffer_formats() const;
		std::vector<D3DFORMAT> get_potential_depth_stencil_formats() const;
		std::vector<D3DDEVTYPE> get_potential_dev_types() const;
		std::vector<UINT> get_potential_present_intervals(bool is_windowed) const;

		bool does_display_mode_pass_constraints(const d3d9_enumerator_setup_params& params, D3DDISPLAYMODE display_mode) const;
		bool does_device_format_pass_constraints(const d3d9_enumerator_setup_params& params, UINT adapter_ordinal, D3DDEVTYPE dev_type, D3DFORMAT adapter_format, D3DFORMAT backbuffer_format) const;

		std::vector<device_info> build_device_infos(
			const d3d9_enumerator_setup_params& params,
			UINT adapter_ordinal,
			const std::vector<D3DFORMAT>& adapter_formats,
			const std::vector<D3DDISPLAYMODE>& adapter_display_modes) const;

		std::vector<device_combo> build_device_combos(
			const d3d9_enumerator_setup_params& params,
			UINT adapter_ordinal,
			const std::vector<D3DFORMAT>& adapter_formats,
			D3DDEVTYPE dev_type,
			const D3DCAPS9& caps,
			bool is_windowed) const;

		std::vector<D3DFORMAT> build_depth_stencil_formats(
			const d3d9_enumerator_setup_params& params,
			UINT adapter_ordinal,
			D3DDEVTYPE dev_type,
			D3DFORMAT adapter_format,
			D3DFORMAT backbuffer_format) const;

		std::vector<UINT> build_present_intervals(const D3DCAPS9& dev_caps, bool is_windowed) const;

		d3d9_enumerator_device_info build_best_device_info_from_user_settings_and_info(const d3d9_user_settings& user_settings, const adapter_info& adapter_info, const device_info& device_info, const device_combo& device_combo, float ranking) const;
		float get_user_settings_to_combo_ranking(const d3d9_user_settings& user_settings, const adapter_info& adapter_info, const device_info& device_info, const device_combo& device_combo) const;
		float get_format_ranking_weight(D3DFORMAT desired_format, D3DFORMAT actual_format, float max_weight) const;
		bool is_adapter_info_nvperfhud(const adapter_info& adapter_info) const;
		float get_display_mode_ranking(const d3d9_user_settings& user_settings, const D3DDISPLAYMODE& display_mode) const;
	};

}