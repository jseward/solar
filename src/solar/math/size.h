#pragma once

#include <ostream>

namespace solar {

	class archive_reader;
	class archive_writer;

	class size {
	private:
		int _width;
		int _height;

	public:
		size();
		size(int w, int h);

		int get_width() const;
		void set_width(int x);

		int get_height() const;
		void set_height(int y);

		bool operator==(const size& rhs) const;
		bool operator!=(const size& rhs) const;
		size operator*(float rhs) const;

	public:
		friend std::ostream& operator<<(std::ostream& os, const size& s);
	};

}