/** @author Sebastian Wild (wild@liverpool.ac.uk) */

#ifndef MERGESORTS_BOTTOM_UP_MERGESORT_H
#define MERGESORTS_BOTTOM_UP_MERGESORT_H

#include "../algorithms.h"
#include "insertionsort.h"
#include "merging.h"

/**
 * Simple bottom-up mergesort implementation.
 * Merging starts after forming runs of length minRunLen.
 * If doSortedCheck is true, we check if two runs are by chance already
 * in sorted order before two runs are merged (compare last of left run with
 * first of right run)
 *
 * @author Sebastian Wild (wild@liverpool.ac.uk)
 */
namespace algorithms {

	template<typename Iterator, unsigned int minRunLen = 24, bool doSortedCheck = true,
	        merging_methods mergingMethod = COPY_BOTH >
	class bottom_up_mergesort final : public sorter<Iterator> {
	private:
		using typename sorter<Iterator>::elem_t;
		using typename sorter<Iterator>::diff_t;
		std::vector<elem_t> _buffer;
	public:

		void sort(Iterator begin, Iterator end) override {
			_buffer.resize(end - begin);
			mergesort(begin, end);
		}

		/** the actual sort; uses [begin,end) */
		void mergesort(Iterator begin, Iterator end) {
			size_t n = end - begin;
			if (minRunLen != 1) {
				Iterator i = begin;
				for (size_t len = minRunLen; i < end; i += len)
					insertionsort(i, std::min(i+len, end));
			}
			for (size_t len = minRunLen; len < n; len *= 2)
				for (Iterator i = begin; i < end - len; i += len + len) {
					Iterator m = i + len;
					if (!doSortedCheck || *(m-1) > *m)
						merge_runs<mergingMethod>(i, m, std::min(i + len + len, end), _buffer.begin());
				}
		}

        std::string name() const override {
            return "BottomUpMergesort+minRunLen=" + std::to_string(minRunLen) +
                   "+checkSorted=" + std::to_string(doSortedCheck) +
                   "+mergingMethod=" + to_string(mergingMethod);
        }
	};

}


#endif //MERGESORTS_BOTTOM_UP_MERGESORT_H
