#include "random_stream.h"

#include "solar/utility/assert.h"
#include "solar/utility/type_convert.h"

namespace solar {

	namespace random_stream_impl {
		static const int IM = 2147483647;
		static const int IA = 16807;
		static const int IQ = 127773;
		static const int IR = 2836;

		static const float NDIV = 1.0f + (int_to_float(IM - 1) / int_to_float(random_stream::NTAB));
		static const float RNMX = 1.0f - 1.2E-07f;
		static const float AM = 1.0f / int_to_float(IM);
	}

	random_stream::random_stream() 
		: _seed(0)
		, _iy(0) {
	}

	random_stream::random_stream(int seed)
		: _seed(0)
		, _iy(0) {

		initialize(seed);
	}

	random_stream::~random_stream() {
	}

	void random_stream::initialize(int seed) {
		ASSERT(seed > 0);
		_seed = seed;

		for (int i = NTAB + 7; i >= 0; i--) {
			iterate_seed();
			if (i < NTAB) {
				_iv.at(i) = _seed;
			}
		}

		_iy = _iv.at(0);
	}

	int random_stream::get_seed() const {
		return _seed;
	}

	float random_stream::generate_float() {
		iterate_seed();

		int i = float_to_int( int_to_float(_iy) / random_stream_impl::NDIV );
		//NOTE: this should not be needed under normal circumstances. The algorithm given combined with floating point trickery should
		//guarantee the index is always within the bounds. For some reason when compiled for the Clr (Managed C++) something changes and
		//at extreme circumstances and causes i to be 32 instead of 31. This happens when:
		//
		// seed = 1133312023
		// iterationCount = 21424665
		i = std::min(i, NTAB - 1);

		_iy = _iv.at(i);
		_iv.at(i) = _seed;

		float f = random_stream_impl::AM * int_to_float(_iy);
		if (f > random_stream_impl::RNMX) {
			f = random_stream_impl::RNMX;
		}
		return f;
	}

	int random_stream::generate_int() {
		return float_to_int(generate_float() * std::numeric_limits<int>::max());
	}

	int random_stream::generate_int(int min_v, int max_v) {
		ASSERT(max_v >= min_v);
		int random_value = min_v + float_to_int(generate_float() * int_to_float(max_v - min_v + 1));
		return std::min(random_value, max_v);
	}

	int random_stream::generate_index(int range_size) {
		if (range_size == 1) {
			return 0;
		}
		else if (range_size > 1) {
			return generate_int(0, range_size - 1);
		}
		return -1;
	}

	void random_stream::iterate_seed() {
		int k = (_seed / random_stream_impl::IQ);
		_seed = (random_stream_impl::IA * (_seed - (k * random_stream_impl::IQ))) - (random_stream_impl::IR * k);
		if (_seed < 0) {
			_seed += random_stream_impl::IM;
		}
	}

} 