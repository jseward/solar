#include "archivable_cloning.h"

#include "solar/utility/assert.h"
#include "solar/utility/unused_parameter.h"
#include "solar/io/memory_stream.h"
#include "binary_archive_reader.h"
#include "binary_archive_writer.h"

namespace solar {

	void clone_archivable_object(archivable& dst, const archivable& src) {
		binary_archive_writer size_calculator(binary_archive_writer::should_calculate_size::YES);
		src.write_to_archive(size_calculator);
		
		unsigned int memory_size = size_calculator.get_calculated_size();
		auto memory = std::unique_ptr<char[]>(new char[memory_size]);

		memory_stream writer_stream = make_writable_memory_stream(memory.get(), memory_size);
		binary_archive_writer writer(writer_stream);
		src.write_to_archive(writer);

		memory_stream reader_stream = make_readable_memory_stream(memory.get(), memory_size);
		binary_archive_reader reader(reader_stream);
		dst.read_from_archive(reader);
	}

}
