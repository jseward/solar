#pragma once

#include "solar/archiving/single_value_archivable.h"
#include "solar/utility/checksum.h"

namespace solar {

	class uv : public single_value_archivable {
	private:
		float _u;
		float _v;

	public:
		uv();
		uv(float u, float v);

		float u() const;
		float v() const;

		bool operator==(const uv& rhs) const;
		bool operator!=(const uv& rhs) const;

		virtual void read_from_archive(archive_reader& reader, const char* name) override;
		virtual void write_to_archive(archive_writer& writer, const char* name) const override;

		checksum to_checksum() const;
	};

}