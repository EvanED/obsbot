#pragma once

#include <cassert>
#include <array>

template <typename T, size_t size_minus_one, size_t tsize = size_minus_one + 1>
class FixedCircularBuffer
{
public:
	typedef unsigned int size_type;
	typedef T const & const_reference;

private:
	std::array<T, tsize + 1> m_storage;
	size_type m_insertion_point = 0;

	static_assert(tsize <= std::numeric_limits<size_type>::max(), "foo");

public:
	FixedCircularBuffer(T const & t) {
		m_storage.fill(t);
	}

	FixedCircularBuffer(T && t) {
		m_storage.fill(t);
	}

	void push_back(T const & v) {
		m_storage[m_insertion_point] = v;
		m_insertion_point = (m_insertion_point + 1) % tsize;
	}

	const_reference at(size_type logical_idx) const {
		if (logical_idx > tsize)
			throw std::out_of_range("Out of range");

		size_type phisical_idx = (logical_idx + m_insertion_point) % tsize;
		return m_storage[physical_idx];
	}

	class const_iterator : std::iterator<std::forward_iterator_tag, T> {
		T const * m_item;
		T const * m_end;

		const_iterator(T const * s, T const * e)
			: m_item(s), m_end(e)
		{}

		friend class FixedCircularBuffer<T, size_minus_one, tsize>;

	public:
		const_iterator& operator++() {
			++m_item;
			if (m_item == m_end)
				m_item = m_end - tsize;
			return *this;
		}

		const_iterator operator++(int) {
			const_iterator copy = *this;
			++*this;
			return copy;
		}

		const_reference operator* () const {
			return *m_item;
		}

		bool operator== (const_iterator that) const {
			assert(this->m_end == that.m_end);
			return this->m_item == that.m_item;
		}

		bool operator!= (const_iterator that) const {
			return !(*this == that);
		}
	};

	const_iterator begin() const {
		return const_iterator(
			m_storage.data() + ((m_insertion_point + 1) % tsize),
			m_storage.data() + tsize);
	}

	const_iterator end() const {
		return const_iterator(
			m_storage.data() + m_insertion_point,
			m_storage.data() + tsize);
	}

	size_type size() const {
		return size_minus_one;
	}
};
