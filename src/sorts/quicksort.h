/** @author Sebastian Wild (wild@liverpool.ac.uk) */


#ifndef MERGESORTS_QUICKSORT_H
#define MERGESORTS_QUICKSORT_H

//#define DEBUG_SORTING

#include <iostream>
#include <algorithm>
#include <functional>
#include <random>
#include "../algorithms.h"
#include "insertionsort.h"
#include "merging.h"

namespace algorithms {

	template<
			typename Iterator,
			unsigned int insertionsortThreshold = 24,
			unsigned int nintherThreshold = 128,
			bool checkSorted = false
	>
	class quicksort final : public sorter<Iterator> {
	private:
		using typename sorter<Iterator>::elem_t;
		using typename sorter<Iterator>::diff_t;
		std::minstd_rand0 rng;
#ifdef DEBUG_SORTING
		Iterator globalBegin, globalEnd;
#endif
	public:

		void sort(Iterator begin, Iterator end) override {
#ifdef DEBUG_SORTING
			globalBegin = begin; globalEnd = end; // for debug
#endif
			quick_sort(begin, end);
		}

#ifdef DEBUG_SORTING
		void debug(Iterator begin, Iterator end) {
			std::cout << " ";
			for (Iterator i = globalBegin; i < globalEnd; ++i) {
				if (begin <= i && i < end)
					std::cout << (i-globalBegin) << "\t";
				else
					std::cout << "  \t";
			}
			std::cout << " \n";

			std::cout << "[";
			for (Iterator i = globalBegin; i < globalEnd; ++i) {
				if (begin <= i && i < end)
					std::cout << *i << "\t";
				else
					std::cout << "  \t";
			}
			std::cout << "]\n\n";
		}
#endif

		/**
		 * sorts [begin,end)
		 */
		void quick_sort(Iterator begin, Iterator end) {
			diff_t n = end - begin;
#ifdef DEBUG_SORTING
			debug(begin, end);
#endif
			if (n <= insertionsortThreshold) return insertionsort(begin, end);

			if (checkSorted && std::is_sorted(begin, end)) return;

			std::uniform_int_distribution<diff_t> d(0,n-1);

			std::__move_median_to_first(
					begin, begin + d(rng), begin + d(rng), begin + d(rng), std::less<>{}
			);
			if (n >= nintherThreshold) {
				std::__move_median_to_first(
						begin+1, begin + d(rng), begin + d(rng), begin + d(rng), std::less<>{}
				);
				std::__move_median_to_first(
						begin+2, begin + d(rng), begin + d(rng), begin + d(rng), std::less<>{}
				);
				std::__move_median_to_first(
						begin, begin, begin + 1, begin + 2, std::less<>{}
				);
			}
			elem_t p = *begin;
#ifdef DEBUG_SORTING
			debug(begin, end);
#endif
			Iterator i = begin, j = end;
			while (true) {
				do { i++; } while (i < end && *i < p);
				do { j--; } while (*j > p);
				if (j > i)
					std::swap(*i, *j);
				else
					break;
			}
			--i;
#ifdef DEBUG_SORTING
			debug(begin, end);
#endif
			std::swap(*i, *begin);
#ifdef DEBUG_SORTING
			debug(begin, end);
#endif
			quick_sort(begin, i);
			quick_sort(i+1, end);
		}

		std::string name() const override {
			return "QuickSort+iscutoff=" + std::to_string(insertionsortThreshold) +
			       "+ninthercutiff=" + std::to_string(nintherThreshold) +
			       "+checkSorted=" + std::to_string(checkSorted);
		}
	};

}

#endif //MERGESORTS_QUICKSORT_H
