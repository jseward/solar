#pragma once

#include <string>
#include <vector>

namespace solar {

	class d3d9_texture;

	class d3d9_texture_pool {
	private:
		const std::string _name;
		const unsigned int _max_size_in_bytes;
		const bool _has_mip_maps;
		std::vector<d3d9_texture*> _textures;
		unsigned int _used_size_in_bytes;

	public:
		d3d9_texture_pool(const std::string& name, unsigned int max_size_in_bytes, bool has_mip_maps);
		~d3d9_texture_pool();
		bool has_mip_maps() const;
		const std::string& get_name() const;
		void add_texture(d3d9_texture* texture, unsigned int texture_size_in_bytes);
		void remove_texture(d3d9_texture* texture, unsigned int texture_size_in_bytes);

	private:
		void remove_oldest_textures();
	};

}