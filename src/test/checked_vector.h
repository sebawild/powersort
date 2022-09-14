//
// Created by seb on 5/22/18.
//

#ifndef MERGESORTS_CHECKED_VECTOR_H
#define MERGESORTS_CHECKED_VECTOR_H

#include <vector>
#include <cstddef>
#include <exception>
#include <string>

/**
 * A simple vector facade that adds boundary checks
 * for all dereference operations.
 * (Note that GNU's debug versions on std::vector etc are too strict;
 * they disallow to use e.g. vec.start() - 1 even as a temporary value ...
 */

struct out_of_bounds_exception : public std::exception {
	std::string message;

	out_of_bounds_exception(size_t index, size_t size) {
		message = std::string("IndexOutOfBounds: ") + std::to_string(index) + " not within [0.." + std::to_string(size) + ") \n";
	}

	const char *what() const noexcept override { return message.c_str(); }
};

template<typename T>
class checked_vector {
private:
	std::vector<T> _vector;
public:

	class iterator : public std::iterator<std::random_access_iterator_tag, T> {
	private:
		using vec_iter = typename std::vector<T>::iterator;
		using diff_t = typename vec_iter::difference_type;
		vec_iter _iter;
		std::vector<T> * _vector;
	public:
		iterator() : _iter(), _vector() {}
		iterator(vec_iter iter, std::vector<T> * vector)
				:_iter(iter), _vector(vector) {}
		iterator(const iterator& other) = default;
		iterator& operator=(const iterator& other) = default;

		iterator& operator++() {++_iter; return *this;}
		iterator operator++(int) { iterator tmp {*this}; operator++(); return tmp; }
		iterator& operator--() {--_iter; return *this;}
		iterator operator--(int) { iterator tmp {*this}; operator--(); return tmp; }
		iterator& operator+=(diff_t d) { _iter += d; return *this; }
		iterator& operator-=(diff_t d) { _iter -= d; return *this; }

		bool operator==(const iterator& rhs) const { return _iter==rhs._iter; }
		bool operator!=(const iterator& rhs) const { return _iter!=rhs._iter; }
		bool operator<(const iterator &rhs) const { return _iter < rhs._iter; }
		bool operator>(const iterator &rhs) const { return rhs < *this; }
		bool operator<=(const iterator &rhs) const { return !(rhs < *this); }
		bool operator>=(const iterator &rhs) const { return !(*this < rhs); }

		iterator operator+(diff_t n) const { return iterator {_iter + n, _vector}; }
		iterator operator-(diff_t n) const { return iterator {_iter - n, _vector}; }
		diff_t operator-(const iterator & rhs) const { return _iter - rhs._iter; }

		T& operator*() const {
			size_t index = _iter - _vector->begin();
			if (index < 0 || index >= _vector->size())
				throw out_of_bounds_exception(index, _vector->size());
			return *_iter;
		}
	};

	explicit checked_vector(const std::vector<T> &_vector) : _vector(_vector) {}

	T &operator[](size_t index) {
		if (index < 0 || index >= _vector.size())
			throw out_of_bounds_exception(index, _vector.size());
		return _vector[index];
	}

	const T &operator[](size_t index) const {
		if (index < 0 || index >= _vector.size())
			throw out_of_bounds_exception(index, _vector.size());
		return _vector[index];
	}

	iterator begin() { return iterator {_vector.begin(), &_vector}; }

//	const_iterator begin() const { return _vector.begin(); }

	iterator end() { return iterator {_vector.end(), &_vector}; }

//	const_iterator end() const { return _vector.end(); }

	bool operator==(checked_vector<T> const & other) const { return _vector == other._vector; }

};

#endif //MERGESORTS_CHECKED_VECTOR_H
