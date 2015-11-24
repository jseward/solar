#include "d3d9_prim2d_lines.h"

#include "solar/utility/assert.h"
#include "solar/utility/unused_parameter.h"
#include "solar/utility/type_convert.h"
#include "d3d9_verify.h"
#include "d3d9_release_com_object.h"

namespace solar {

	d3d9_prim2d_lines::d3d9_prim2d_lines(d3d9_context& context)
		: _context(context) 
		, _d3dx_line(nullptr)
		, _is_rendering(false) {
	}

	d3d9_prim2d_lines::~d3d9_prim2d_lines() {
		ASSERT(_d3dx_line == nullptr);
		ASSERT(!_is_rendering);
	}

	void d3d9_prim2d_lines::setup() {
		_context.add_device_event_handler(this);
	}

	void d3d9_prim2d_lines::teardown() {
		_context.remove_device_event_handler(this);
	}

	bool d3d9_prim2d_lines::is_rendering() const {
		return _is_rendering;
	}

	void d3d9_prim2d_lines::begin_rendering(float line_width) {
		ASSERT(!_is_rendering);
		_is_rendering = true;
		 
		D3D9_VERIFY(_context.get_device()->GetViewport(&_viewport));
		D3D9_VERIFY(_d3dx_line->SetWidth(line_width));
		D3D9_VERIFY(_d3dx_line->SetAntialias(TRUE));
		D3D9_VERIFY(_d3dx_line->Begin());
	}

	void d3d9_prim2d_lines::end_rendering() {
		ASSERT(_is_rendering);
		_is_rendering = false;

		D3D9_VERIFY(_d3dx_line->End());
	}

	void d3d9_prim2d_lines::render_segments(const vec2* points, unsigned int point_count, const color& color) {
		ASSERT(_is_rendering);

		if (point_count > 1) {
			static_assert(sizeof(solar::vec2) == sizeof(D3DXVECTOR2), "solar::vec2 cannot be casted to D3DXVECTOR2");
			auto d3dx_points = reinterpret_cast<const D3DXVECTOR2*>(points);

			if (_viewport.X != 0 || _viewport.Y != 0) {
				//NOTE: ID3DXLine will internally transform all the points by the viewport's top left position. This causes problems because
				//all clients expect the points to be in screen space, not in the current viewport space.
				D3DMATRIX old_projection;
				D3D9_VERIFY(_context.get_device()->GetTransform(D3DTS_PROJECTION, &old_projection));
				D3DXMATRIX new_projection;
				::D3DXMatrixTranslation(&new_projection, -uint_to_float(_viewport.X), -uint_to_float(_viewport.Y), 0.f);
				new_projection *= old_projection;
				D3D9_VERIFY(_context.get_device()->SetTransform(D3DTS_PROJECTION, &new_projection));
				D3D9_VERIFY(_d3dx_line->Draw(d3dx_points, point_count, color.to_argb32()));
				D3D9_VERIFY(_context.get_device()->SetTransform(D3DTS_PROJECTION, &old_projection));
			}
			else {
				D3D9_VERIFY(_d3dx_line->Draw(d3dx_points, point_count, color.to_argb32()));
			}
		}
	}

	void d3d9_prim2d_lines::on_device_created(IDirect3DDevice9* device) {
		ASSERT(_d3dx_line == nullptr);
		D3D9_VERIFY(::D3DXCreateLine(device, &_d3dx_line));
	}

	void d3d9_prim2d_lines::on_device_released(IDirect3DDevice9*) {
		d3d9_release_com_object(_d3dx_line);
	}

	void d3d9_prim2d_lines::on_device_reset(IDirect3DDevice9*) {
		_d3dx_line->OnResetDevice();
	}

	void d3d9_prim2d_lines::on_device_lost(IDirect3DDevice9*) {
		_d3dx_line->OnLostDevice();
	}

}