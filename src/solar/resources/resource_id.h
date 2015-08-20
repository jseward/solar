#pragma once

#include "solar/archiving/single_value_archivable.h"
#include "solar/archiving/archiving_helpers.h"

namespace solar {

	template<typename ResourceT, typename FactoryT>
	class resource_id : public single_value_archivable {
	private:
		static FactoryT* _s_factory;

	public:
		static void set_factory(FactoryT* factory);

	private:
		std::string _id;
		std::string _id_source_description;

		mutable ResourceT* _cached_resource;
		mutable int _cached_resource_context;

	public:
		resource_id();
		virtual ~resource_id();
		
		resource_id& set_id(const std::string& id, const std::string& id_source_description);

		bool is_id_empty() const;
		ResourceT& get() const;
		operator ResourceT&();
		ResourceT* operator->();

		bool operator==(const resource_id& rhs) const;

		virtual void read_from_archive(archive_reader& reader, const char* name) override;
		virtual void write_to_archive(archive_writer& writer, const char* name) const override;

	private:
		virtual int get_current_resource_caching_context(const FactoryT& factory) const = 0;
		virtual ResourceT* get_uncached_resource(FactoryT& factory, const std::string& id, const std::string& id_source_description) const = 0;
	};

	template<typename ResourceT, typename FactoryT>
	void resource_id<ResourceT, FactoryT>::set_factory(FactoryT* factory) {
		_s_factory = factory;
	}

	template<typename ResourceT, typename FactoryT>
	resource_id<ResourceT, FactoryT>::resource_id() 
		: _cached_resource(nullptr)
		, _cached_resource_context(-1) {
	}

	template<typename ResourceT, typename FactoryT>
	resource_id<ResourceT, FactoryT>::~resource_id() {
	}

	template<typename ResourceT, typename FactoryT>
	resource_id<ResourceT, FactoryT>& resource_id<ResourceT, FactoryT>::set_id(const std::string& id, const std::string& id_source_description) {
		_id = id;
		_id_source_description = id_source_description;
		_cached_resource = nullptr;
		_cached_resource_context = -1;
		return *this;
	}

	template<typename ResourceT, typename FactoryT>
	bool resource_id<ResourceT, FactoryT>::is_id_empty() const {
		return _id.empty();
	}

	template<typename ResourceT, typename FactoryT>
	ResourceT& resource_id<ResourceT, FactoryT>::get() const {
		ASSERT(_s_factory != nullptr);
		int current_resource_context = get_current_resource_caching_context(*_s_factory);
		if (_cached_resource_context != current_resource_context) {
			_cached_resource_context = current_resource_context;
			_cached_resource = get_uncached_resource(*_s_factory, _id, _id_source_description);
			ASSERT(_cached_resource != nullptr); //not allowed to return a null resource. all resource implementations must gracefully handle empty objects.
		}
		return *_cached_resource;
	}

	template<typename ResourceT, typename FactoryT>
	resource_id<ResourceT, FactoryT>::operator ResourceT&() {
		return get();
	}

	template<typename ResourceT, typename FactoryT>
	ResourceT* resource_id<ResourceT, FactoryT>::operator->() {
		return &get();
	}

	template<typename ResourceT, typename FactoryT>
	bool resource_id<ResourceT, FactoryT>::operator==(const resource_id& rhs) const {
		return _id == rhs._id;
	}

	template<typename ResourceT, typename FactoryT>
	void resource_id<ResourceT, FactoryT>::read_from_archive(archive_reader& reader, const char* name) {
		read_string(reader, name, _id);
		_id_source_description = reader.get_source_description();
	}

	template<typename ResourceT, typename FactoryT>
	void resource_id<ResourceT, FactoryT>::write_to_archive(archive_writer& writer, const char* name) const {
		write_string(writer, name, _id);
	}

}