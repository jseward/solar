#pragma once

#include "solar/archiving/single_value_archivable.h"
#include "solar/archiving/archiving_helpers.h"
#include "resource_id_caching_context.h"
#include "resource_factory_caching_context.h"

namespace solar {

	template<typename ResourceT, typename FactoryT>
	class resource_id : public single_value_archivable {
	private:
		static FactoryT* _s_factory;

	public:
		static void set_factory(FactoryT* factory);
		static const FactoryT& get_factory();

	private:
		std::string _id;
		std::string _id_source_description;

		mutable ResourceT* _cached_resource;
		mutable resource_id_caching_context _caching_context;

	public:
		resource_id();
		resource_id(const char* id, const char* id_source_description);
		virtual ~resource_id();
		
		resource_id& set_id(const std::string& id, const std::string& id_source_description);

		bool is_id_empty() const;
		ResourceT& get() const;
		ResourceT* try_get_ptr() const;
		ResourceT* operator->() const;
		operator ResourceT*() const; //implicitely convert to ptr safetly (will return nullptr if id is empty)

		bool operator==(const resource_id& rhs) const;

		virtual void read_from_archive(archive_reader& reader, const char* name) override;
		virtual void write_to_archive(archive_writer& writer, const char* name) const override;

	private:
		virtual const resource_factory_caching_context& get_factory_caching_context(const FactoryT& factory) const = 0;
		virtual ResourceT* get_uncached_resource(FactoryT& factory, const std::string& id, const std::string& id_source_description) const = 0;
	};

	template<typename ResourceT, typename FactoryT>
	void resource_id<ResourceT, FactoryT>::set_factory(FactoryT* factory) {
		_s_factory = factory;
	}

	template<typename ResourceT, typename FactoryT>
	const FactoryT& resource_id<ResourceT, FactoryT>::get_factory() {
		return *_s_factory;
	}

	template<typename ResourceT, typename FactoryT>
	resource_id<ResourceT, FactoryT>::resource_id() 
		: _cached_resource(nullptr)
		, _caching_context() {
	}

	template<typename ResourceT, typename FactoryT>
	resource_id<ResourceT, FactoryT>::resource_id(const char* id, const char* id_source_description)
		: _id(id)
		, _id_source_description(id_source_description)
		, _cached_resource(nullptr)
		, _caching_context() {
	}

	template<typename ResourceT, typename FactoryT>
	resource_id<ResourceT, FactoryT>::~resource_id() {
	}

	template<typename ResourceT, typename FactoryT>
	resource_id<ResourceT, FactoryT>& resource_id<ResourceT, FactoryT>::set_id(const std::string& id, const std::string& id_source_description) {
		_id = id;
		_id_source_description = id_source_description;
		_cached_resource = nullptr;
		_caching_context.reset();
		return *this;
	}

	template<typename ResourceT, typename FactoryT>
	bool resource_id<ResourceT, FactoryT>::is_id_empty() const {
		return _id.empty();
	}

	template<typename ResourceT, typename FactoryT>
	ResourceT& resource_id<ResourceT, FactoryT>::get() const {
		ASSERT(_s_factory != nullptr);
		int factory_caching_context_value = get_factory_caching_context(*_s_factory).get_value();
		if (_caching_context.get_value() != factory_caching_context_value) {
			_caching_context.set_value(factory_caching_context_value);
			_cached_resource = get_uncached_resource(*_s_factory, _id, _id_source_description);
			ASSERT(_cached_resource != nullptr); //not allowed to return a null resource. all resource implementations must gracefully handle empty objects.
		}
		return *_cached_resource;
	}

	template<typename ResourceT, typename FactoryT>
	ResourceT* resource_id<ResourceT, FactoryT>::operator->() const {
		return &get();
	}

	template<typename ResourceT, typename FactoryT>
	ResourceT* resource_id<ResourceT, FactoryT>::try_get_ptr() const {
		if (is_id_empty()) {
			return nullptr;
		}
		return &get();
	}

	template<typename ResourceT, typename FactoryT>
	resource_id<ResourceT, FactoryT>::operator ResourceT*() const {
		return try_get_ptr();
	}

	template<typename ResourceT, typename FactoryT>
	bool resource_id<ResourceT, FactoryT>::operator==(const resource_id& rhs) const {
		return _id == rhs._id;
	}

	template<typename ResourceT, typename FactoryT>
	void resource_id<ResourceT, FactoryT>::read_from_archive(archive_reader& reader, const char* name) {
		read_string(reader, name, _id);
		_id_source_description = reader.get_source_description();
		_cached_resource = nullptr;
		_caching_context.reset();
	}

	template<typename ResourceT, typename FactoryT>
	void resource_id<ResourceT, FactoryT>::write_to_archive(archive_writer& writer, const char* name) const {
		write_string(writer, name, _id);
	}

}