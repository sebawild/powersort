/** @author Sebastian Wild (wild@liverpool.ac.uk) */

#ifndef MERGESORTS_TOP_DOWN_MERGESORT_H
#define MERGESORTS_TOP_DOWN_MERGESORT_H

#include "../algorithms.h"
#include "merging.h"
#include "insertionsort.h"

/**
 * Simple top-down mergesort implementation.
 *
 * Recursion is stopped at subproblems of sizes at most insertionsortThreshold;
 * those are sorted by straight insertion sort.
 * If doSortedCheck is true, we check if two runs are by chance already
 * in sorted order before two runs are merged (compare last of left run with
 * first of right run).
 *
 * @author Sebastian Wild (wild@liverpool.ac.uk)
 */
namespace algorithms {

	template<typename Iterator, unsigned int insertionsortThreshold = 24, bool doSortedCheck = true,
	        merging_methods mergingMethod = COPY_BOTH>
	class top_down_mergesort final : public sorter<Iterator>
	{
	private:
		using typename sorter<Iterator>::elem_t;
		using typename sorter<Iterator>::diff_t;
		std::vector<elem_t> _buffer;
	public:

		void sort(Iterator begin, Iterator end) override
		{
			_buffer.resize(end - begin);
			mergesort(begin, end);
		}

		/** the actual sort; uses [begin,end) */
		void mergesort(Iterator begin, Iterator end)
		{
			diff_t n = end - begin;
			if (n <= insertionsortThreshold)
				return insertionsort(begin, end);
			Iterator m = begin + (n >> 1);
			mergesort(begin, m);
			mergesort(m, end);
			if (!doSortedCheck || *(m-1) > *m)
				merge_runs<mergingMethod>(begin, m, end, _buffer.begin());
		}

        std::string name() const override {
            return "TopDownMergesort+iscutoff=" + std::to_string(insertionsortThreshold) +
                   "+checkSorted=" + std::to_string(doSortedCheck) +
                   "+mergingMethod=" + to_string(mergingMethod);
        }
	};

}

#endif //MERGESORTS_TOP_DOWN_MERGESORT_H
