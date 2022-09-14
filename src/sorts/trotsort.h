/** @author Sebastian Wild (wild@liverpool.ac.uk) */

#ifndef MERGESORTS_TROTSORT_H
#define MERGESORTS_TROTSORT_H

#include <algorithm>
#include <iterator>
#include "timsort.h"
#include "insertionsort.h"
#include "merging.h"


namespace algorithms {


	template <
			typename RandomAccessIterator,
	        bool useBinaryInsertionsort,
            merging_methods mergingMethod = COPY_BOTH
	> class TrotSort {
		typedef RandomAccessIterator iter_t;
		typedef typename std::iterator_traits<iter_t>::value_type value_t;
		typedef typename std::iterator_traits<iter_t>::difference_type diff_t;

		static const int MIN_MERGE = 32;

		std::vector<value_t> buffer_; // temp storage for merges

		struct run {
			iter_t base;
			diff_t len;

			run(iter_t const b, diff_t const l) : base(b), len(l) {
			}
		};
		std::vector<run> pending_;

	public:
		static void sort(iter_t const begin, iter_t const end) {
			assert(begin <= end);

			size_t nRemaining = (end - begin);
			if (nRemaining < 2) {
				return; // nothing to do
			}

			if (nRemaining < MIN_MERGE) {
				diff_t const initRunLen = countRunAndMakeAscending(begin, end);
				smallSort(begin, end, begin + initRunLen);
				return;
			}

			TrotSort ts(nRemaining);
			auto const minRun = static_cast<const size_t>(minRunLength(nRemaining));
			iter_t cur = begin;
			do {
				diff_t runLen = countRunAndMakeAscending(cur, end);

				if (runLen < minRun) {
					diff_t const force = std::min(nRemaining, minRun);
					smallSort(cur, cur + force, cur + runLen);
					runLen = force;
				}

				ts.pushRun(cur, runLen);
				ts.mergeCollapse();

				cur += runLen;
				nRemaining -= runLen;
			} while (nRemaining != 0);

			assert(cur == end);
			ts.mergeForceCollapse();
			assert(ts.pending_.size() == 1);

		} // sort()

	private:
		static inline void smallSort(iter_t const lo, iter_t const hi, iter_t start) {
			if (useBinaryInsertionsort)
				algorithms::binary_insertionsort(lo, hi, start);
			else
				algorithms::insertionsort(lo, hi, start);
		}


		static diff_t countRunAndMakeAscending(iter_t const lo, iter_t const hi) {
			assert(lo < hi);
			iter_t runHi = lo + 1;
			if (runHi == hi) return 1;

			if (*lo > *(runHi++)) { // descending
				while (runHi < hi && *(runHi - 1) > *runHi ) ++runHi;
				std::reverse(lo, runHi);
			} else { // ascending
				while (runHi < hi && *(runHi - 1) <= *runHi ) ++runHi;
			}
			return runHi - lo;
		}

		static diff_t minRunLength(diff_t n) {
			assert(n >= 0);
			diff_t r = 0;
			while (n >= MIN_MERGE) {
				r |= (n & 1);
				n >>= 1;
			}
			return n + r;
		}

		TrotSort(size_t len) {
			/*
			 * Allocate runs-to-be-merged stack (which cannot be expanded).  The
			 * stack length requirements are described in listsort.txt.  The C
			 * version always uses the same stack length (85), but this was
			 * measured to be too expensive when sorting "mid-sized" arrays (e.g.,
			 * 100 elements) in Java.  Therefore, we use smaller (but sufficiently
			 * large) stack lengths for smaller arrays.  The "magic numbers" in the
			 * computation below must be changed if MIN_MERGE is decreased.  See
			 * the MIN_MERGE declaration above for more information.
			 * The maximum value of 49 allows for an array up to length
			 * Integer.MAX_VALUE-4, if array is filled by the worst case stack size
			 * increasing scenario. More explanations are given in section 4 of:
			 * http://envisage-project.eu/wp-content/uploads/2015/02/sorting.pdf
			 */
			size_t stackLen = (len <    120  ?  5 :
			                   len <   1542  ? 10 :
			                   len < 119151  ? 24 : 49);
			pending_.reserve(stackLen);
			buffer_.resize(len);
		}

		void pushRun(iter_t const runBase, diff_t const runLen) {
			pending_.push_back(run(runBase, runLen));
		}

		void mergeCollapse() {
			while (pending_.size() > 1) {
				diff_t n = pending_.size() - 2;

				if ((n > 0 && pending_[n - 1].len <= pending_[n].len + pending_[n + 1].len) ||
				    (n > 1 && pending_[n - 2].len <= pending_[n - 1].len + pending_[n].len)) {
					if (pending_[n - 1].len < pending_[n + 1].len) {
						--n;
					}
					mergeAt(n);
				} else if (pending_[n].len <= pending_[n + 1].len) {
					mergeAt(n);
				} else {
					break;
				}
			}
		}

		void mergeForceCollapse() {
			while (pending_.size() > 1) {
				diff_t n = pending_.size() - 2;

				if (n > 0 && pending_[n - 1].len < pending_[n + 1].len) {
					--n;
				}
				mergeAt(n);
			}
		}

		void mergeAt(diff_t const i) {
			size_t const stackSize = pending_.size();
			assert(stackSize >= 2);
			assert(i >= 0);
			assert(i == stackSize - 2 || i == stackSize - 3);

			iter_t base1 = pending_[i].base;
			diff_t len1  = pending_[i].len;
			iter_t base2 = pending_[i + 1].base;
			diff_t len2  = pending_[i + 1].len;

			assert(len1 > 0 && len2 > 0);
			assert(base1 + len1 == base2);

			pending_[i].len = len1 + len2;

			if (i == stackSize - 3) {
				pending_[i + 1] = pending_[i + 2];
			}

			pending_.pop_back();

			// Merge remaining runs, using tmp array with min(len1, len2) elements
			merge_runs<mergingMethod>(base1, base2, base2 + len2, buffer_.begin());

		}

	};



	/**
	 * Timsort as ported here from JDK, with galloping removed
	 * https://github.com/gfx/cpp-TimSort/blob/master/timsort.hpp
	 *
	 * @author Sebastian Wild (wild@uwaterloo.ca)
	 */
	template<typename Iterator,bool useBinaryInsertionsort = false, merging_methods mergingMethod = COPY_SMALLER>
	class trotsort final : public sorter<Iterator> {
	public:
		void sort(Iterator begin, Iterator end) override {
			typedef typename std::iterator_traits<Iterator>::value_type value_type;
			TrotSort<Iterator,useBinaryInsertionsort, mergingMethod>::sort(begin, end);
		}

		std::string name() const override {
			return std::string("TimsortTrot") +
					std::string("-useBinaryInsertionsort=") + std::to_string(useBinaryInsertionsort);
		}
	};


}

#endif //MERGESORTS_TROTSORT_H
