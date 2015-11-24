#pragma once

#include <array>
#include "solar/containers/fixed_vector.h"
#include "vec2.h"
#include "ray2.h"

namespace solar {

	class tri2_closest_position_info {
	public:
		vec2 _position_on_edge;
		float _distance_squared;
		bool _is_within;

	public:
		tri2_closest_position_info();
	};

	class tri2_closest_edge_info {
	public:
		vec2 _position_on_edge;
		float _distance_squared;
		int _edge_index;
		float _edge_t;

	public:
		tri2_closest_edge_info();
	};

	class tri2 {
	private:
		using vertex_array = std::array<vec2, 3>;
		using edge_ray_vector = fixed_vector<ray2, 3>;

	private:
		vertex_array _vertices;

		mutable edge_ray_vector _cached_edge_rays;

	public:
		tri2();
		tri2(const vec2& v0, const vec2& v1, const vec2& v2);

		tri2_closest_position_info get_closest_position(const vec2& p) const;
		tri2_closest_edge_info get_closest_position_on_any_edge(const vec2& p) const;
		tri2_closest_edge_info get_closest_position_on_edge(const vec2& p, int edge_index) const;

		bool is_position_within(const vec2& p) const;
		bool is_position_within(tri2_closest_position_info& cpi, const vec2& p) const;

	private:
		enum class edge_side { NONE, LEFT, RIGHT };
		edge_side get_edge_side(const vec2& p, int edge_index) const;
		ray2 make_edge_ray(int edge_index) const;
	};

}