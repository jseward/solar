#include "brush.h"
#include "solar/archiving/archiving_helpers.h"
#include "solar/archiving/archiving_math_helpers.h"

namespace solar {

	brush::brush(const char* texture_pool_name)
		: _texture_id(texture_pool_name) {
	}

	const std::string& brush::get_id() const {
		return _id;
	}

	const brush_stretch_margins& brush::get_stretch_margins() const {
		return _stretch_margins;
	}

	texture& brush::get_texture() const {
		return _texture_id.get();
	}

	const simple_rect_uvs& brush::get_uvs(brush_stretch_region region) const {
		if (!_has_cached_uvs) {
			cache_uvs();
			_has_cached_uvs = true;
		}
		return _cached_uvs.at(region);
	}

	void brush::cache_uvs() const {
		if (_texel_box.get_width() == 0 || _texel_box.get_height() == 0) {
			ALERT("brush has zero width or height texel_box : {}", _id);
		}

		auto texture_size = _texture_id.get().request_size();
		if (
			_texel_box.get_left() < 0 || 
			_texel_box.get_top() < 0 ||
			_texel_box.get_right() > texture_size._width ||
			_texel_box.get_bottom() > texture_size._height ) {
			
			ALERT("brush texel_box not valid for texture size.\n\nbrush_id : {}\ntexture_size : {}\ntexel_box : {}", _id, texture_size, _texel_box);
		}

		if (!_stretch_margins.can_build_region_rect(_texel_box)) {
			ALERT("brush stretch_margins not valid for texel_box.\n\nbrush_id : {}\nstretch_margins : {}\ntexel_box : {}", _id, _stretch_margins, _texel_box);
		}
		else {
			FOR_EACH_ENUM(brush_stretch_region, region) {
				_cached_uvs.at(region) = simple_rect_uvs(texture_size, _stretch_margins.build_region_rect(_texel_box, region));
			}
		}
	}

	void brush::read_from_archive(archive_reader& reader) {
		read_string(reader, "id", _id);
		read_object(reader, "texture_id", _texture_id);
		read_rect(reader, "texel_box", _texel_box);
		read_object(reader, "stretch_margins", _stretch_margins);

		_has_cached_uvs = false;
	}

	void brush::write_to_archive(archive_writer& writer) const {
		write_string(writer, "id", _id);
		write_object(writer, "texture_id", _texture_id);
		write_rect(writer, "texel_box", _texel_box);
		write_object(writer, "stretch_margins", _stretch_margins);
	}

	std::ostream& operator<<(std::ostream& os, const brush& brush) {
		return os << brush.get_id();
	}
}