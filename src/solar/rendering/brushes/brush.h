#pragma once

#include <string>
#include "solar/math/rect.h"
#include "solar/archiving/archivable.h"
#include "solar/rendering/textures/texture_id.h"
#include "solar/rendering/textures/simple_rect_uvs.h"
#include "solar/containers/enum_array.h"
#include "brush_stretch_margins.h"
#include "brush_stretch_region.h"

namespace solar {

	class brush : public archivable {
	private:
		std::string _id;
		texture_id _texture_id;
		rect _texel_box;
		brush_stretch_margins _stretch_margins;

		mutable bool _has_cached_uvs;
		mutable enum_array<simple_rect_uvs, brush_stretch_region> _cached_uvs;

	public:
		brush(const char* texture_pool_name);

		const std::string& get_id() const;
		const brush_stretch_margins& get_stretch_margins() const;
		texture& get_texture() const;
		const simple_rect_uvs& get_uvs(brush_stretch_region region) const;

	public:
		virtual void read_from_archive(archive_reader& reader) override;
		virtual void write_to_archive(archive_writer& writer) const override;

	private:
		void cache_uvs() const;

	public:
		friend std::ostream& operator<<(std::ostream& os, const brush& brush);
	};

}