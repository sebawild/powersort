/** @author Sebastian Wild (wild@liverpool.ac.uk) */

#ifndef MERGESORTS_PEEKSORT_H
#define MERGESORTS_PEEKSORT_H

//#define DEBUG_SORTING

#include <iostream>
#include "../algorithms.h"
#include "insertionsort.h"
#include "merging.h"
#include <vector>

namespace algorithms {

	/**
	 * Implementation of peeksort as described in “Nearly optimal mergesorts”
	 * (https://www.wild-inter.net/publications/munro-wild-2018).
	 *
	 * If the subproblem has size at most insertionsortThreshold,
	 * it is sorted by straight insertion sort instead of merging.
	 * If onlyIncreasingRuns is true, we only find weakly increasing runs
	 * while peeking into the middle. That simplifies run detection a bit,
	 * but it does not detect descending runs.
	 *
	 * @author Sebastian Wild (wild@liverpool.ac.uk)
	 */
	template<typename Iterator, unsigned int insertionsortThreshold = 24, bool onlyIncreasingRuns = false,
	        merging_methods mergingMethod = COPY_BOTH>
	class peeksort final : public sorter<Iterator> {
	private:
		using typename sorter<Iterator>::elem_t;
		using typename sorter<Iterator>::diff_t;
		std::vector<elem_t> _buffer;
#ifdef DEBUG_SORTING
		Iterator globalBegin, globalEnd;
#endif
	public:

		void sort(Iterator begin, Iterator end) override {
			_buffer.resize(end - begin);
#ifdef DEBUG_SORTING
			globalBegin = begin; globalEnd = end; // for debug
#endif
			peek_sort(begin, end, begin + 1, end - 1);
		}

#ifdef DEBUG_SORTING
		void debug(Iterator begin, Iterator end, Iterator leftRunEnd, Iterator rightRunBegin, Iterator m) {
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
			std::cout << "]\n";

			std::cout << " ";
			for (Iterator i = globalBegin; i < globalEnd; ++i) {
				if (begin <= i && i < leftRunEnd)
					std::cout << "L" << "\t";
				else if (rightRunBegin <= i && i < end)
					std::cout << "R" << "\t";
				else std::cout << "  \t";
			}
			std::cout << " \n";

			std::cout << " ";
			for (Iterator i = globalBegin; i < globalEnd; ++i) {
				if (i == m - 1)
					std::cout << " |\t";
				else if (i == m)
					std::cout << "m\t";
				else std::cout << "  \t";
			}
			std::cout << " \n\n";
		}
#endif

		/**
		 * sorts [begin,end), assuming that [begin,leftRunEnd) and
		 * [rightRunBegin,end) are sorted
		 */
		void peek_sort(Iterator begin, Iterator end, Iterator leftRunEnd, Iterator rightRunBegin) {
			if (leftRunEnd == end || rightRunBegin == begin) return;

			size_t n = end - begin;
			if (n <= insertionsortThreshold)
				return insertionsort(begin, end, leftRunEnd);
			Iterator m = begin + (n >> 1); // middle split between m and m-1
#ifdef DEBUG_SORTING
			debug(begin, end, leftRunEnd, rightRunBegin, m);
#endif
			if (m <= leftRunEnd) {
				// |XXXXXXXX|XX     X|
				peek_sort(leftRunEnd, end, leftRunEnd + 1, rightRunBegin);
				merge_runs<mergingMethod>(begin, leftRunEnd, end, _buffer.begin());
			} else if (m >= rightRunBegin) {
				// |XX     X|XXXXXXXX|
				peek_sort(begin, rightRunBegin, leftRunEnd, rightRunBegin-1);
				merge_runs<mergingMethod>(begin, rightRunBegin, end, _buffer.begin());
			} else {
				// find middle run, i.e., run containing m-1
				Iterator i, j;
				if (onlyIncreasingRuns) {
					//   m-2  m-1 | m  m+1
					//    2    3  | 1   2
					//    iXXXXXX |
					//      | XXXXXXXX  j
					i = weaklyIncreasingSuffix(leftRunEnd, m);
					j = weaklyIncreasingPrefix(m-1, rightRunBegin);
				} else {
					if (*(m-1) <= *m) {
						i = weaklyIncreasingSuffix(leftRunEnd, m);
						j = weaklyIncreasingPrefix(m-1, rightRunBegin);
					} else {
						i = strictlyDecreasingSuffix(leftRunEnd, m);
						j = strictlyDecreasingPrefix(m-1, rightRunBegin);
						std::reverse(i,j);
					}
				}
				if (i == begin && j == end) return; // single run
				if (m - i < j - m) {
					// |XX     x|xxxx   X|
					peek_sort(begin, i, leftRunEnd, i-1);
					peek_sort(i, end, j, rightRunBegin);
					merge_runs<mergingMethod>(begin, i, end, _buffer.begin());
				} else {
					// |XX   xxx|x      X|
					peek_sort(begin, j, leftRunEnd, i);
					peek_sort(j, end, j+1, rightRunBegin);
					merge_runs<mergingMethod>(begin, j, end, _buffer.begin());
				}
			}

		}

		std::string name() const override {
			return "PeekSort+iscutoff=" + std::to_string(insertionsortThreshold) +
			       "+onlyIncRuns=" + std::to_string(onlyIncreasingRuns) +
                   "+mergingMethod=" + to_string(mergingMethod);
		}
	};

}


#endif //MERGESORTS_PEEKSORT_H
