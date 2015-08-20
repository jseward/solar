#pragma once

namespace solar {

	class archive_reader;
	class archive_writer;

	class single_value_archivable {
	public:
		virtual void read_from_archive(archive_reader& reader, const char* name) = 0;
		virtual void write_to_archive(archive_writer& writer, const char* name) const = 0;
	};

}