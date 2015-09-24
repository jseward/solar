#include "tri2.h"

namespace solar {

	tri2_closest_position_info::tri2_closest_position_info() 
		: _position_on_edge()
		, _distance_squared(0.f)
		, _is_within(false) {
	}

	tri2_closest_edge_info::tri2_closest_edge_info()
		: _position_on_edge()
		, _distance_squared(0.f)
		, _edge_index(0)
		, _edge_t(0.f) {
	}

	tri2::tri2() {
	}

	tri2::tri2(const vec2& v0, const vec2& v1, const vec2& v2) {
		_vertices[0] = v0;
		_vertices[1] = v1;
		_vertices[2] = v2;
	}

	tri2_closest_position_info tri2::get_closest_position(const vec2& p) const {
		//DO NOT BE TEMPTED TO USE THESE!!! THEY ARE NOT STABLE AND OVERKILL FOR 2D
		//http://www.geometrictools.com/Documentation/DistancePoint3Triangle3.pdf and http://www.geometrictools.com/LibMathematics/Distance/Distance.html.
		//There are numerical stability problems with this algorithm (and its a bitch to debug and expand): http://www.geometrictools.com/Downloads/KnownProblems.html

		const auto cei = get_closest_position_on_any_edge(p);

		tri2_closest_position_info cpi;
		cpi._position_on_edge = cei._position_on_edge;
		
		if (cei._distance_squared < .0001f) {
			cpi._distance_squared = 0.f;
			cpi._is_within = true;
		}
		else {
			cpi._distance_squared = cei._distance_squared;
			cpi._is_within = true;
			for (int edge_i = 0; edge_i < 3; ++edge_i) {
				if (get_edge_side(p, edge_i) == edge_side::LEFT) {
					cpi._is_within = false;
					break;
				}
			}
		}

		return cpi;
	}

	tri2_closest_edge_info tri2::get_closest_position_on_any_edge(const vec2& p) const {
		tri2_closest_edge_info cei;
		cei._distance_squared = std::numeric_limits<float>::max();
		for (int edge_i = 0; edge_i < 3; ++edge_i) {
			tri2_closest_edge_info cei_at_i = get_closest_position_on_edge(p, edge_i);
			if (cei_at_i._distance_squared < cei._distance_squared) {
				cei = cei_at_i;
			}
		}
		return cei;
	}

	tri2_closest_edge_info tri2::get_closest_position_on_edge(const vec2& p, int edge_index) const {
		const auto ray = make_edge_ray(edge_index);

		tri2_closest_edge_info cei;
		cei._edge_t = ray.get_closest_t_constrained(p);
		cei._position_on_edge = ray.get_position_on_ray(cei._edge_t);
		cei._distance_squared = get_distance_squared(p, cei._position_on_edge);
		cei._edge_index = edge_index;
		return cei;
	}

	bool tri2::is_position_within(const vec2& p) const {
		tri2_closest_position_info cpi;
		return is_position_within(cpi, p);
	}

	bool tri2::is_position_within(tri2_closest_position_info& cpi, const vec2& p) const {
		cpi = get_closest_position(p);
		return cpi._is_within;
	}

	tri2::edge_side tri2::get_edge_side(const vec2& p, int edge_index) const {
		const vec2& a = _vertices[edge_index % 3];
		const vec2& b = _vertices[(edge_index + 1) % 3];
		float det = ((b._x - a._x) * (p._y - a._y) - (b._y - a._y) * (p._x - a._x));

		if (det > 0.f) {
			return edge_side::LEFT;
		}
		else if (det < 0.f) {
			return edge_side::RIGHT;
		}
		else {
			return edge_side::NONE;
		}
	}

	ray2 tri2::make_edge_ray(int edge_index) const {
		if (_cached_edge_rays.empty()) {
			for (int i = 0; i < 3; ++i) {
				_cached_edge_rays.push_back(make_ray2_begin_end(_vertices[i], _vertices[(i + 1) % 3]));
			}
		}
		return _cached_edge_rays[edge_index];
	}

}