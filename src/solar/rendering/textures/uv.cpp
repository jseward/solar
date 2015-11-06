#include "uv.h"

#include "solar/archiving/archiving_helpers.h"

namespace solar {

	uv::uv() 
		: _u(0.f)
		, _v(0.f) {
	}

	uv::uv(float u, float v)
		: _u(u)
		, _v(v) {
	}

	bool uv::operator==(const uv& rhs) const {
		return 
			_u == rhs._u && 
			_v == rhs._v;
	}

	bool uv::operator!=(const uv& rhs) const {
		return !(*this == rhs);
	}

	void uv::read_from_archive(archive_reader& reader, const char* name) {
		read_floats(reader, name, _u, _v);
	}

	void uv::write_to_archive(archive_writer& writer, const char* name) const {
		write_floats(writer, name, _u, _v);
	}

	checksum uv::to_checksum() const {
		return checksum()
			.add_checksum_at_index(0, checksum().add_float(_u))
			.add_checksum_at_index(1, checksum().add_float(_v));
	}
}