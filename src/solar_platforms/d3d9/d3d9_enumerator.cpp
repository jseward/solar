#include "d3d9_enumerator.h"

#include <algorithm>
#include "d3d9_format_helpers.h"
#include "d3d9_verify.h"
#include "solar/utility/assert.h"
#include "solar/utility/trace.h"
#include "solar/containers/container_helpers.h"

namespace solar {

	namespace d3d9_enumerator_impl {
		bool sort_display_modes_predicate(const D3DDISPLAYMODE& a, const D3DDISPLAYMODE& b) {
			if (a.Width != b.Width) { return a.Width > b.Width; }
			if (a.Height != b.Height) { return a.Height > b.Height; }
			if (a.Format != b.Format) { return a.Format > b.Format; }
			if (a.RefreshRate != b.RefreshRate) { return a.RefreshRate > b.RefreshRate; }
			return false;
		}

		int get_display_mode_difference_score(const d3d9_user_settings& user_settings, const D3DDISPLAYMODE& display_mode) {
			return
				std::abs(static_cast<int>(display_mode.Width) - user_settings.get_backbuffer_size().get_width()) +
				std::abs(static_cast<int>(display_mode.Height) - user_settings.get_backbuffer_size().get_height()) +
				std::abs(static_cast<int>(display_mode.RefreshRate) - static_cast<int>(user_settings.get_fullscreen_refresh_rate()));
		}

		int get_depth_stencil_format_difference_score(const d3d9_user_settings& user_settings, D3DFORMAT format) {
			return
				std::abs(get_depth_format_bit_count(format) - get_depth_format_bit_count(user_settings.get_depth_stencil_format())) +
				std::abs(get_stencil_format_bit_count(format) - get_stencil_format_bit_count(user_settings.get_depth_stencil_format())) +
				(format != user_settings.get_depth_stencil_format()) ? 1 : 0;
		}
	}

	d3d9_enumerator::d3d9_enumerator() {
	}

	void d3d9_enumerator::setup(const d3d9_enumerator_setup_params& params) {
		ASSERT(_adapter_infos.empty());
		for (UINT adapter_ordinal = 0; adapter_ordinal < params.get_IDirect3D9()->GetAdapterCount(); adapter_ordinal++) {
			_adapter_infos.push_back(build_adapter_info(params, adapter_ordinal));
		}

		TRACE("d3d9_enumerator found {0} adapters...", _adapter_infos.size());
		for (auto adapter_info : _adapter_infos) {
			TRACE("[{0}] {1} : {2}", adapter_info._adapter_ordinal, adapter_info._adapter_identifier.DeviceName, adapter_info._adapter_identifier.Description);
		}
	}

	d3d9_enumerator_device_info d3d9_enumerator::build_best_device_info_from_user_settings(IDirect3D9* idirect3d9, const d3d9_user_settings& user_settings) const {
		
		d3d9_enumerator_device_info best_device_info;
		
		for (const auto& adapter_info : _adapter_infos) {
			
			if (adapter_info._adapter_ordinal != user_settings.get_adapter_ordinal()) {
				//skip this adapter, wrong ordinal
			}
			else {

				D3DDISPLAYMODE adapter_desktop_display_mode;
				D3D9_VERIFY(idirect3d9->GetAdapterDisplayMode(adapter_info._adapter_ordinal, &adapter_desktop_display_mode));

				for (const auto& device_info : adapter_info._device_infos) {
					for (const auto& combo : device_info._combos) {

						if (combo._is_windowed && combo._adapter_format != adapter_desktop_display_mode.Format) {
							//skip this combo, doesn't match desktop
						}
						else if (combo._is_windowed != d3d9_window_type_details::is_device_windowed(user_settings.get_window_type())) {
							//skip this combo, doesn't match is_windowed
						}
						else {
							float ranking = get_user_settings_to_combo_ranking(user_settings, adapter_info, device_info, combo);
							if (ranking > best_device_info.get_ranking()) {
								best_device_info = build_best_device_info_from_user_settings_and_info(user_settings, adapter_info, device_info, combo, ranking);
							}
						}

					}
				}
			}
		}

		return best_device_info;
	}

	d3d9_enumerator::adapter_info d3d9_enumerator::build_adapter_info(const d3d9_enumerator_setup_params& params, UINT adapter_ordinal) {
		
		adapter_info adapter_info;
		adapter_info._adapter_ordinal = adapter_ordinal;
		D3D9_VERIFY(params.get_IDirect3D9()->GetAdapterIdentifier(adapter_ordinal, 0, &adapter_info._adapter_identifier));
		
		std::vector<D3DFORMAT> adapter_formats;

		for (D3DFORMAT format : get_potential_adapter_formats()) {
			for (UINT mode_index = 0; mode_index < params.get_IDirect3D9()->GetAdapterModeCount(adapter_ordinal, format); ++mode_index) {
				
				D3DDISPLAYMODE display_mode;
				D3D9_VERIFY(params.get_IDirect3D9()->EnumAdapterModes(adapter_ordinal, format, mode_index, &display_mode));
				if (does_display_mode_pass_constraints(params, display_mode)) {
					adapter_info._display_modes.push_back(display_mode);
					push_back_if_not_found(adapter_formats, display_mode.Format);
				}

			}
		}
		
		sort(adapter_info._display_modes, d3d9_enumerator_impl::sort_display_modes_predicate);

		D3DDISPLAYMODE adapter_display_mode;
		D3D9_VERIFY(params.get_IDirect3D9()->GetAdapterDisplayMode(adapter_ordinal, &adapter_display_mode));
		push_back_if_not_found(adapter_formats, adapter_display_mode.Format);

		adapter_info._device_infos = build_device_infos(params, adapter_ordinal, adapter_formats, adapter_info._display_modes);

		return adapter_info;
	}

	std::vector<D3DFORMAT> d3d9_enumerator::get_potential_adapter_formats() const {
		std::vector<D3DFORMAT> formats;
		formats.push_back(D3DFMT_X8R8G8B8);
		formats.push_back(D3DFMT_X1R5G5B5);
		formats.push_back(D3DFMT_R5G6B5);
		formats.push_back(D3DFMT_A2R10G10B10);
		return formats;
	}

	std::vector<D3DFORMAT> d3d9_enumerator::get_potential_backbuffer_formats() const {
		std::vector<D3DFORMAT> formats;
		formats.push_back(D3DFMT_A8R8G8B8);
		formats.push_back(D3DFMT_X8R8G8B8);
		formats.push_back(D3DFMT_A2R10G10B10);
		formats.push_back(D3DFMT_R5G6B5);
		formats.push_back(D3DFMT_A1R5G5B5);
		formats.push_back(D3DFMT_X1R5G5B5);
		return formats;
	}

	std::vector<D3DFORMAT> d3d9_enumerator::get_potential_depth_stencil_formats() const {
		std::vector<D3DFORMAT> formats;
		formats.push_back(D3DFMT_D16);
		formats.push_back(D3DFMT_D15S1);
		formats.push_back(D3DFMT_D24X8);
		formats.push_back(D3DFMT_D24S8);
		formats.push_back(D3DFMT_D24X4S4);
		formats.push_back(D3DFMT_D32);
		return formats;
	}

	std::vector<D3DDEVTYPE> d3d9_enumerator::get_potential_dev_types() const {
		std::vector<D3DDEVTYPE> dev_types;
		dev_types.push_back(D3DDEVTYPE_HAL);
		dev_types.push_back(D3DDEVTYPE_SW);
		dev_types.push_back(D3DDEVTYPE_REF);
		return dev_types;
	}

	std::vector<UINT> d3d9_enumerator::get_potential_present_intervals(bool is_windowed) const {
		std::vector<UINT> present_intervals;
		present_intervals.push_back(D3DPRESENT_INTERVAL_IMMEDIATE);
		present_intervals.push_back(D3DPRESENT_INTERVAL_DEFAULT);
		present_intervals.push_back(D3DPRESENT_INTERVAL_ONE);
		if (!is_windowed)
		{
			present_intervals.push_back(D3DPRESENT_INTERVAL_TWO);
			present_intervals.push_back(D3DPRESENT_INTERVAL_THREE);
			present_intervals.push_back(D3DPRESENT_INTERVAL_FOUR);
		}
		return present_intervals;
	}

	bool d3d9_enumerator::does_display_mode_pass_constraints(const d3d9_enumerator_setup_params& params, D3DDISPLAYMODE display_mode) const {
		return
			display_mode.Width >= params.get_min_display_mode_width() &&
			display_mode.Height >= params.get_min_display_mode_height();
	}

	bool d3d9_enumerator::does_device_format_pass_constraints(const d3d9_enumerator_setup_params& params, UINT adapter_ordinal, D3DDEVTYPE dev_type, D3DFORMAT adapter_format, D3DFORMAT backbuffer_format) const {
		if (params.get_is_post_pixelshader_blending_required()) {
			if (FAILED(params.get_IDirect3D9()->CheckDeviceFormat(adapter_ordinal, dev_type, adapter_format, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING, D3DRTYPE_TEXTURE, backbuffer_format))) {
				return false;
			}
		}
		return true;
	}

	std::vector<d3d9_enumerator::device_info> d3d9_enumerator::build_device_infos(
		const d3d9_enumerator_setup_params& params,
		UINT adapter_ordinal,
		const std::vector<D3DFORMAT>& adapter_formats,
		const std::vector<D3DDISPLAYMODE>& adapter_display_modes) const {

		std::vector<device_info> device_infos;

		for (D3DDEVTYPE dev_type : get_potential_dev_types()) {

			std::vector<device_combo> combos;

			D3DCAPS9 caps;
			if (!FAILED(params.get_IDirect3D9()->GetDeviceCaps(adapter_ordinal, dev_type, &caps)))
			{
				push_back_range(combos, build_device_combos(params, adapter_ordinal, adapter_formats, dev_type, caps, true));
				if (!adapter_display_modes.empty()) {
					push_back_range(combos, build_device_combos(params, adapter_ordinal, adapter_formats, dev_type, caps, false));
				}
			}

			if (!combos.empty()) {
				device_info device_info;
				device_info._dev_type = dev_type;
				device_info._caps = caps;
				device_info._combos = combos;
				device_infos.push_back(device_info);
			}
		}

		return device_infos;
	}

	std::vector<d3d9_enumerator::device_combo> d3d9_enumerator::build_device_combos(
		const d3d9_enumerator_setup_params& params,
		UINT adapter_ordinal,
		const std::vector<D3DFORMAT>& adapter_formats,
		D3DDEVTYPE dev_type,
		const D3DCAPS9& caps,
		bool is_windowed) const {

		std::vector<device_combo> device_combos;

		for (D3DFORMAT adapter_format : adapter_formats) {
			for (D3DFORMAT backbuffer_format : get_potential_backbuffer_formats()) {
				
				if (FAILED(params.get_IDirect3D9()->CheckDeviceType(adapter_ordinal, dev_type, adapter_format, backbuffer_format, is_windowed))) {
					continue;
				}

				if (!does_device_format_pass_constraints(params, adapter_ordinal, dev_type, adapter_format, backbuffer_format)) {
					continue;
				}

				auto depth_stencil_formats = build_depth_stencil_formats(params, adapter_ordinal, dev_type, adapter_format, backbuffer_format);
				auto present_intervals = build_present_intervals(caps, is_windowed);

				if (!depth_stencil_formats.empty() && !present_intervals.empty()) {
					device_combo combo;
					combo._adapter_format = adapter_format;
					combo._backbuffer_format = backbuffer_format;
					combo._is_windowed = is_windowed;
					combo._depth_stencil_formats = depth_stencil_formats;
					combo._present_intervals = present_intervals;
					device_combos.push_back(combo);
				}

			}
		}

		return device_combos;
	}

	std::vector<D3DFORMAT> d3d9_enumerator::build_depth_stencil_formats(
		const d3d9_enumerator_setup_params& params,
		UINT adapter_ordinal,
		D3DDEVTYPE dev_type,
		D3DFORMAT adapter_format,
		D3DFORMAT backbuffer_format) const
	{
		std::vector<D3DFORMAT> depth_stencil_formats;

		for (D3DFORMAT depth_stencil_format : get_potential_depth_stencil_formats()) {
			if (SUCCEEDED(params.get_IDirect3D9()->CheckDeviceFormat(adapter_ordinal, dev_type, adapter_format, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, depth_stencil_format))) {
				if (SUCCEEDED(params.get_IDirect3D9()->CheckDepthStencilMatch(adapter_ordinal, dev_type, adapter_format, backbuffer_format, depth_stencil_format))) {
					depth_stencil_formats.push_back(depth_stencil_format);
				}
			}
		}

		return depth_stencil_formats;
	}

	std::vector<UINT> d3d9_enumerator::build_present_intervals(const D3DCAPS9& dev_caps, bool is_windowed) const {

		std::vector<UINT> present_intervals;

		for (UINT present_interval : get_potential_present_intervals(is_windowed)) {
			if (
				(present_interval == D3DPRESENT_INTERVAL_DEFAULT) ||
				((dev_caps.PresentationIntervals & present_interval) != 0)) {

				present_intervals.push_back(present_interval);
			}
		}

		return present_intervals;
	}

	float d3d9_enumerator::get_user_settings_to_combo_ranking(
		const d3d9_user_settings& user_settings, 
		const adapter_info& adapter_info, 
		const device_info& device_info, 
		const device_combo& device_combo) const {

		float ranking = 0.f;

		if (is_adapter_info_nvperfhud(adapter_info)) {
			ranking += 1000.f;
		}

		if (device_info._dev_type == D3DDEVTYPE_HAL) {
			ranking += 100.f;
		}

		if ((device_info._caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) != 0) {
			ranking += 1.f;
		}

		ranking += get_format_ranking_weight(user_settings.get_adapter_format(), device_combo._adapter_format, 1.f);
		ranking += get_format_ranking_weight(user_settings.get_backbuffer_format(), device_combo._backbuffer_format, 1.f);

		if (any(device_combo._depth_stencil_formats, user_settings.get_depth_stencil_format())) {
			ranking += 1.f;
		}

		if (!device_combo._is_windowed) {
			auto matching_display_modes = copy_if(adapter_info._display_modes, [&device_combo](const D3DDISPLAYMODE& dm) { return dm.Format == device_combo._adapter_format; });

			if (any_if(matching_display_modes, [&user_settings](const D3DDISPLAYMODE& dm) { return size(dm.Width, dm.Height) == user_settings.get_backbuffer_size(); })) {
				ranking += 1.f;
			}

			if (any_if(matching_display_modes, [&user_settings](const D3DDISPLAYMODE& dm) { return dm.RefreshRate == user_settings.get_fullscreen_refresh_rate(); })) {
				ranking += 1.f;
			}
		}

		return ranking;
	}

	float d3d9_enumerator::get_format_ranking_weight(D3DFORMAT desired_format, D3DFORMAT actual_format, float max_weight) const {
		if (actual_format == desired_format) {
			return max_weight;
		}

		int desired_bits = get_color_channel_bit_count(desired_format);
		int actual_bits = get_color_channel_bit_count(actual_format);
		int bits_diff = abs(desired_bits - actual_bits);
		return max_weight * std::max(0.f, (0.9f - ((float)bits_diff / (float)desired_bits)));
	}

	d3d9_enumerator_device_info d3d9_enumerator::build_best_device_info_from_user_settings_and_info(const d3d9_user_settings& user_settings, const adapter_info& adapter_info, const device_info& device_info, const device_combo& device_combo, float ranking) const {
	
		D3DDEVTYPE dev_type = is_adapter_info_nvperfhud(adapter_info) ? 
			D3DDEVTYPE_REF : 
			D3DDEVTYPE_HAL;

		DWORD behavior_flags = ((device_info._caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) != 0) ?
			D3DCREATE_HARDWARE_VERTEXPROCESSING :
			D3DCREATE_SOFTWARE_VERTEXPROCESSING;

		D3DPRESENT_PARAMETERS pp = { 0 };
		pp.Windowed = device_combo._is_windowed;
		pp.BackBufferFormat = device_combo._backbuffer_format;
		pp.BackBufferCount = 2;
		pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		pp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
		if (device_combo._is_windowed)
		{
			pp.BackBufferWidth = user_settings.get_backbuffer_size().get_width();
			pp.BackBufferHeight = user_settings.get_backbuffer_size().get_height();
		}
		else
		{
			auto best_display_mode = sort_and_get_first(adapter_info._display_modes, [&user_settings](const D3DDISPLAYMODE& a, const D3DDISPLAYMODE& b) {
				return
					d3d9_enumerator_impl::get_display_mode_difference_score(user_settings, a) <
					d3d9_enumerator_impl::get_display_mode_difference_score(user_settings, b);
			});

			pp.BackBufferWidth = best_display_mode.Width;
			pp.BackBufferHeight = best_display_mode.Height;
			pp.FullScreen_RefreshRateInHz = best_display_mode.RefreshRate;
		}

		pp.MultiSampleType = D3DMULTISAMPLE_NONE;
		pp.MultiSampleQuality = 0;

		pp.EnableAutoDepthStencil = true;
		pp.AutoDepthStencilFormat = sort_and_get_first(device_combo._depth_stencil_formats, [&user_settings](D3DFORMAT a, D3DFORMAT b) {
			return
				d3d9_enumerator_impl::get_depth_stencil_format_difference_score(user_settings, a) <
				d3d9_enumerator_impl::get_depth_stencil_format_difference_score(user_settings, b);
		});

		pp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;

		return d3d9_enumerator_device_info()
			.set_is_valid(true)
			.set_ranking(ranking)
			.set_adapter_ordinal(adapter_info._adapter_ordinal)
			.set_adapter_format(device_combo._adapter_format)
			.set_device_type(dev_type)
			.set_behavior_flags(behavior_flags)
			.set_present_parameters(pp);
	}

	bool d3d9_enumerator::is_adapter_info_nvperfhud(const adapter_info& adapter_info) const {
		return ::strstr(adapter_info._adapter_identifier.Description, "PerfHUD") != 0;
	}
}