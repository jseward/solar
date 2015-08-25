#pragma once

#include <array>

namespace solar {
	
	//Based on http://www.phys.uni.torun.pl/nrbook/c7-1.pdf - rand1()

	class random_stream {
	public:
		enum { NTAB = 32 };

	private:
		int _seed;
		int _iy;
		std::array<int, NTAB> _iv;

	public:
		random_stream();
		random_stream(const random_stream&) = delete;
		random_stream& operator=(const random_stream&) = delete;
		virtual ~random_stream();

		void initialize(int seed);

		int get_seed() const;

		float generate_float();
		int generate_int();
		int generate_int(int min_v, int max_v);
		int generate_index(int range_size);

		template<typename Con> typename Con::iterator generate_random_iterator(Con& con);
		
	private:
		void iterate_seed();
	};

	template<typename Con> typename Con::iterator random_stream::generate_random_iterator(Con& con) {
		if (con.size() == 0) {
			return con.end();
		}

		int index = generate_index(con.size());
		return con.begin() + index;
	}

}