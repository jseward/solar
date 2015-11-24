#pragma once

//normally use std containers when possible but much easier to control memory of this linked_list as opposed to std::list. it has been used for
//two shipped products now so pretty safe.

#include "solar/utility/assert.h"

namespace solar {

	template<typename T>
	class linked_list_node {
	public:
		linked_list_node<T>* _prev;
		linked_list_node<T>* _next;
		T _data;

	public:
		linked_list_node() 
			: _prev(nullptr)
			, _next(nullptr) {
		}
	};

	template<typename T>
	class linked_list_iterator : public std::iterator<std::bidirectional_iterator_tag, T> {
	private:
		linked_list_node<T>* _node;
		int _list_snapshot;

	public:
		linked_list_iterator() 
			: _node(nullptr)
			, _list_snapshot(-1) {
		}

		linked_list_iterator(linked_list_node<T>* node, int list_snapshot)
			: _node(node)
			, _list_snapshot(list_snapshot) {
		}

		linked_list_iterator(const linked_list_iterator& copy)
			: _node(copy._node)
			, _list_snapshot(copy._list_snapshot) {
		}

		linked_list_iterator& operator=(const linked_list_iterator& copy) {
			_node = copy._node;
			_list_snapshot = copy._list_snapshot;
			return *this;
		}

		bool operator==(const linked_list_iterator& rhs) const {
			ASSERT(_list_snapshot == rhs._list_snapshot);
			return _node == rhs._node;
		}

		bool operator!=(const linked_list_iterator& rhs) const {
			ASSERT(_list_snapshot == rhs._list_snapshot);
			return _node != rhs._node;
		}

		linked_list_iterator& operator++() {
			ASSERT(_node != nullptr);
			_node = _node->_next;
			return *this;
		}

		linked_list_iterator& operator--() {
			ASSERT(_node != nullptr);
			_node = _node->_prev;
			return *this;
		}

		T& operator*() const {
			return _node->_data;
		}

		T* operator->() const {
			return &_node->_data;
		}

	private:
		template<typename T> friend class linked_list_const_iterator; //so const_iterator can be constructed by iterator
	};

	template<typename T>
	class linked_list_const_iterator : public std::iterator<std::bidirectional_iterator_tag, T> {
	private:
		linked_list_node<T>* _node;
		int _list_snapshot;

	public:
		linked_list_const_iterator()
			: _node(nullptr)
			, _list_snapshot(-1) {
		}

		linked_list_const_iterator(linked_list_node<T>* node, int list_snapshot)
			: _node(node)
			, _list_snapshot(list_snapshot) {
		}

		linked_list_const_iterator(const linked_list_iterator<T>& copy)
			: _node(copy._node)
			, _list_snapshot(copy._list_snapshot) {
		}

		linked_list_const_iterator(const linked_list_const_iterator& copy)
			: _node(copy._node)
			, _list_snapshot(copy._list_snapshot) {
		}

		linked_list_const_iterator& operator=(const linked_list_const_iterator& copy) {
			_node = copy._node;
			_list_snapshot = copy._list_snapshot;
			return *this;
		}

		bool operator==(const linked_list_const_iterator& rhs) const {
			ASSERT(_list_snapshot == rhs._list_snapshot);
			return _node == rhs._node;
		}

		bool operator!=(const linked_list_const_iterator& rhs) const {
			ASSERT(_list_snapshot == rhs._list_snapshot);
			return _node != rhs._node;
		}

		linked_list_const_iterator& operator++() {
			ASSERT(_node != nullptr);
			_node = _node->_next;
			return *this;
		}

		linked_list_const_iterator& operator--() {
			ASSERT(_node != nullptr);
			_node = _node->_prev;
			return *this;
		}

		const T& operator*() const {
			return _node->_data;
		}

		const T* operator->() const {
			return &_node->_data;
		}

		linked_list_node<T>* get_node() {
			return _node;
		}
	};

	template<typename T, typename NodeFactoryT>
	class linked_list {
	public:
		typedef T value_type;
		typedef size_t size_type;
		typedef linked_list_iterator<T> iterator;
		typedef linked_list_const_iterator<T> const_iterator;
		typedef linked_list_node<T> node_type;

	private:
		static NodeFactoryT* _s_node_factory;

	public:
		static void set_node_factory(NodeFactoryT* node_factory) {
			_s_node_factory = node_factory;
		}

	private:
		node_type* _first;
		node_type* _last;
		size_type _size;
		int _iterator_snapshot;

	public:
		linked_list()
			: _first(nullptr)
			, _last(nullptr)
			, _size(0)
			, _iterator_snapshot(0) {
		}

		virtual ~linked_list() {
			clear();
		}
		
		bool empty() const {
			return _size == 0;
		}

		size_type size() const {
			return _size;
		}

		iterator begin() {
			return iterator(_first, _iterator_snapshot);
		}

		iterator end() {
			return iterator(nullptr, _iterator_snapshot);
		}

		const_iterator begin() const {
			return const_iterator(_first, _iterator_snapshot);
		}

		const_iterator end() const {
			return const_iterator(nullptr, _iterator_snapshot);
		}

		void clear() {
			erase(begin(), end());
		}

		iterator erase(const_iterator position) {
			_iterator_snapshot++;

			auto node = position.get_node();
			ASSERT(node != nullptr);

			if (node == _first) {
				_first = node->_next;
			}

			if (node == _last) {
				_last = node->_prev;
			}

			if (node->_prev != nullptr) {
				node->_prev->_next = node->_next;
			}

			if (node->_next != nullptr) {
				node->_next->_prev = node->_prev;
			}

			auto next_node = node->_next;
			release_node(node);
			return iterator(next_node, _iterator_snapshot);
		}

		iterator erase(const_iterator first, const_iterator last) {
			const_iterator curr = first;
			while (curr != last) {
				const_iterator temp = curr;
				++curr;
				erase(temp);
			}
			return iterator(last.get_node(), _iterator_snapshot);
		}

		void push_back(const T& value) {
			insert(end(), value);
		}

		iterator insert(const_iterator position, const T& value) {
			auto new_node = create_node();
			new_node->_data = value;

			auto position_node = position.get_node();
			if (position_node != nullptr) {
				new_node->_next = position_node;
				new_node->_prev = position_node->_prev;
				if (position_node->_prev != nullptr) {
					position_node->_prev->_next = new_node;
				}
				position_node->_prev = new_node;
			}
			else {
				if (_last != nullptr) {
					ASSERT(_last->_next == nullptr);
					_last->_next = new_node;
					new_node->_prev = _last;
				}
				_last = new_node;
			}

			if (_first == position_node) {
				_first = new_node;
			}

			_iterator_snapshot++;
			return iterator(new_node, _iterator_snapshot);
		}

	private:
		node_type* create_node() {
			_size++;
			ASSERT(_s_node_factory != nullptr);
			return _s_node_factory->create_node();
		}

		void release_node(node_type* node) {
			ASSERT(_size > 0);
			_size--;
			ASSERT(_s_node_factory != nullptr);
			_s_node_factory->release_node(node);
		}
	};

}