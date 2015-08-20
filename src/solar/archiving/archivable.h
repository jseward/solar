#pragma once

namespace solar {

	class archive_reader;
	class archive_writer;

	class archivable {
	public:
		virtual void read_from_archive(archive_reader& reader) = 0;
		virtual void write_to_archive(archive_writer& writer) const = 0;
	};

}