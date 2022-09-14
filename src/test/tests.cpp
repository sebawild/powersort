//
// Created by seb on 5/20/18.
//

#include <cmath>
#include "gtest/gtest.h"
#include "sorter_harness.h"
#include "checked_vector.h"
#include "welford.h"
#include "sorts/peeksort.h"
#include "sorts/powersort.h"
#include "sorts/timsort.h"
#include "sorts/trotsort.h"
#include "sorts/quicksort.h"
#include "sorts/merging.h"
#include "sorts/merging_multiway.h"
#include "sorts/insertionsort.h"
#include "sorts/top_down_mergesort.h"
#include "sorts/bottom_up_mergesort.h"
#include "sorts/powersort_4way.h"

std::random_device rd;
inputs::RNG rng(rd());

template<typename Container>
void print(Container const & c) {
	std::cout << "[ ";
	for (auto && x : c) std::cout << x << " ";
	std::cout << "]" << std::endl;
}
template<typename Iter>
void print(Iter start, Iter end) {
	std::cout << "[ ";
	for (Iter i = start; i != end; ++i) std::cout << *i << " ";
	std::cout << "]" << std::endl;
}


struct MergingTest : public ::testing::Test {
	//                                              0  1   2   3   4   5  6  7  8  9    10
	checked_vector<int> v{std::vector<int> {10, 20, 30, 40, 120, 5, 6, 7, 8, 50, 100}};
	checked_vector<int> v2{std::vector<int> {5, 10, 20, 30, 40, 120, 6, 7, 8, 50, 100}};
	checked_vector<int> buffer{std::vector<int>(11)};
	checked_vector<int> buffer_big{std::vector<int>(11+4)};

	checked_vector<int> v_sorted{std::vector<int> {5, 6, 7, 8, 10, 20, 30, 40, 50, 100, 120}};
};

TEST_F(MergingTest, bitonicMergeExample) {
	auto a = v.begin();
	auto a2 = v2.begin();
	auto b = buffer.begin();
	algorithms::merge_runs_bitonic(a, a + 5, a + 11, b);
	ASSERT_TRUE(std::is_sorted(v.begin(),v.end()));
	ASSERT_EQ(v, v_sorted);
	algorithms::merge_runs_bitonic(a2, a2 + 6, a2 + 11, b);
	ASSERT_TRUE(std::is_sorted(v2.begin(),v2.end()));
	ASSERT_EQ(v2, v_sorted);
}

TEST_F(MergingTest, bitonicBranchlessMergeExample) {
	auto a = v.begin();
    auto a2 = v2.begin();
	auto b = buffer.begin();
	algorithms::merge_runs_bitonic_branchless(a, a + 5, a + 11, b);
	ASSERT_TRUE(std::is_sorted(v.begin(),v.end()));
	ASSERT_EQ(v, v_sorted);
    algorithms::merge_runs_bitonic_branchless(a2, a2 + 6, a2 + 11, b);
    ASSERT_TRUE(std::is_sorted(v2.begin(),v2.end()));
    ASSERT_EQ(v2, v_sorted);
}

TEST_F(MergingTest, bitonicManualCopyMergeExample) {
	auto a = v.begin();
    auto a2 = v2.begin();
	auto b = buffer.begin();
	algorithms::merge_runs_bitonic_manual_copy(a, a + 5, a + 11, b);
	ASSERT_TRUE(std::is_sorted(v.begin(),v.end()));
	ASSERT_EQ(v, v_sorted);
    algorithms::merge_runs_bitonic_manual_copy(a2, a2 + 6, a2 + 11, b);
    ASSERT_TRUE(std::is_sorted(v2.begin(),v2.end()));
    ASSERT_EQ(v2, v_sorted);
}

TEST_F(MergingTest, copySmallerRunExample) {
	auto a = v.begin();
    auto a2 = v2.begin();
	auto b = buffer.begin();
	algorithms::merge_runs_copy_half(a, a + 5, a + 11, b);
	ASSERT_TRUE(std::is_sorted(v.begin(),v.end()));
	ASSERT_EQ(v, v_sorted);
    algorithms::merge_runs_copy_half(a2, a2 + 6, a2 + 11, b);
    ASSERT_TRUE(std::is_sorted(v2.begin(),v2.end()));
    ASSERT_EQ(v2, v_sorted);
}

TEST_F(MergingTest, mergeBasicExample) {
	auto a = v.begin();
    auto a2 = v2.begin();
	auto b = buffer.begin();
	algorithms::merge_runs_basic(a, a + 5, a + 11, b);
	ASSERT_TRUE(std::is_sorted(v.begin(),v.end()));
	ASSERT_EQ(v, v_sorted);
    algorithms::merge_runs_basic(a2, a2 + 6, a2 + 11, b);
    ASSERT_TRUE(std::is_sorted(v2.begin(),v2.end()));
    ASSERT_EQ(v2, v_sorted);
}

TEST_F(MergingTest, mergeBasicSentinelExample) {
	auto a = v.begin();
    auto a2 = v2.begin();
	auto b = buffer_big.begin();
	algorithms::merge_runs_basic_sentinels(a, a + 5, a + 11, b);
	ASSERT_TRUE(std::is_sorted(v.begin(),v.end()));
	ASSERT_EQ(v, v_sorted);
    algorithms::merge_runs_basic_sentinels(a2, a2 + 6, a2 + 11, b);
    ASSERT_TRUE(std::is_sorted(v2.begin(),v2.end()));
    ASSERT_EQ(v2, v_sorted);
}



TEST_F(MergingTest, fourwayWillemEmptyLastRuns) {
	auto a = v.begin();
	auto b = buffer_big.begin();
	algorithms::merge_4runs_numeric(a, a + 5, a + 11, a+11, a+11, b);
	ASSERT_TRUE(std::is_sorted(v.begin(),v.end()));
	ASSERT_EQ(v, v_sorted);
}
TEST_F(MergingTest, fourwayWillemEmptyFirstRuns) {
	auto a = v.begin();
	auto b = buffer_big.begin();
	algorithms::merge_4runs_numeric(a, a, a, a + 5, a + 11, b);
	ASSERT_TRUE(std::is_sorted(v.begin(),v.end()));
	ASSERT_EQ(v, v_sorted);
}
TEST_F(MergingTest, fourwayWillemEmptyMiddleRuns) {
	auto a = v.begin();
	auto b = buffer_big.begin();
    algorithms::merge_4runs_numeric(a, a + 5, a + 5, a + 5, a + 11, b);
	ASSERT_TRUE(std::is_sorted(v.begin(),v.end()));
	ASSERT_EQ(v, v_sorted);
}


TEST_F(MergingTest, fourwayEmptyLastRuns) {
	auto a = v.begin();
	auto b = buffer_big.begin();
	algorithms::merge_4runs_indices(a, a + 5, a + 11, a+11, a+11, b);
	ASSERT_TRUE(std::is_sorted(v.begin(),v.end()));
	ASSERT_EQ(v, v_sorted);
}
TEST_F(MergingTest, fourwayEmptyFirstRuns) {
	auto a = v.begin();
	auto b = buffer_big.begin();
	algorithms::merge_4runs_indices(a, a, a, a + 5, a + 11, b);
	ASSERT_TRUE(std::is_sorted(v.begin(),v.end()));
	ASSERT_EQ(v, v_sorted);
}
TEST_F(MergingTest, fourwayEmptyMiddleRuns) {
	auto a = v.begin();
	auto b = buffer_big.begin();
    algorithms::merge_4runs_indices(a, a + 5, a + 5, a + 5, a + 11, b);
	ASSERT_TRUE(std::is_sorted(v.begin(),v.end()));
	ASSERT_EQ(v, v_sorted);
}



TEST_F(MergingTest, fourwayStagesEmptyLastRuns) {
	auto a = v.begin();
	auto b = buffer_big.begin();
	algorithms::merge_4runs_by_stages(a, a + 5, a + 11, a+11, a+11, b);
	ASSERT_TRUE(std::is_sorted(v.begin(),v.end()));
	ASSERT_EQ(v, v_sorted);
}
TEST_F(MergingTest, fourwayStagesEmptyFirstRuns) {
	auto a = v.begin();
	auto b = buffer_big.begin();
	algorithms::merge_4runs_by_stages(a, a, a, a + 5, a + 11, b);
	ASSERT_TRUE(std::is_sorted(v.begin(),v.end()));
	ASSERT_EQ(v, v_sorted);
}
TEST_F(MergingTest, fourwayStagesEmptyMiddleRuns) {
	auto a = v.begin();
	auto b = buffer_big.begin();
    algorithms::merge_4runs_by_stages(a, a + 5, a + 5, a + 5, a + 11, b);
	ASSERT_TRUE(std::is_sorted(v.begin(),v.end()));
	ASSERT_EQ(v, v_sorted);
}


TEST_F(MergingTest, weaklyIncreasingLeft) {
	auto runEnd = algorithms::weaklyIncreasingPrefix(v.begin(), v.end());
	ASSERT_EQ(runEnd, v.begin()+5);
	auto runStart = algorithms::weaklyIncreasingSuffix(v.begin(), v.end());
	ASSERT_EQ(runStart, v.begin()+5);
	auto decEnd = algorithms::strictlyDecreasingPrefix(v.begin(), v.end());
	ASSERT_EQ(decEnd, v.begin()+1);
	auto decBeg = algorithms::strictlyDecreasingSuffix(v.begin(), v.end());
	ASSERT_EQ(decBeg, v.end()-1);

	std::reverse(v.begin(), v.end());
	// 0    1    2  3  4  5   6   7   8   9   10
	// 120, 100, 8, 7, 6, 5 , 50, 40, 30, 20, 10
	decEnd = algorithms::strictlyDecreasingPrefix(v.begin(), v.end());
	ASSERT_EQ(decEnd, v.begin()+6);
	decBeg = algorithms::strictlyDecreasingSuffix(v.begin(), v.end());
	ASSERT_EQ(decBeg, v.begin()+6);

	std::sort(v.begin(), v.end());
	ASSERT_EQ(algorithms::weaklyIncreasingPrefix(v.begin(), v.end()), v.end());
	ASSERT_EQ(algorithms::weaklyIncreasingSuffix(v.begin(), v.end()), v.begin());
}











TEST(insertionsort, example) {
	checked_vector<long> v { std::vector<long> {5,8,0,3,2,-2,4,7,9} };
	algorithms::insertionsort(v.begin(), v.end(), 2);
	ASSERT_TRUE(std::is_sorted(v.begin(),v.end()));
}


TEST(insertionsort, exampleBinary) {
	checked_vector<long> v { std::vector<long> {5,8,0,3,2,-2,4,7,9} };
	algorithms::binary_insertionsort(v.begin(), v.end(), 2);
	ASSERT_TRUE(std::is_sorted(v.begin(),v.end()));
}



























using vec_iter = typename checked_vector<int>::iterator;



TEST(inputs, testNewRandomPerm) {
	int n = 10;
	int *pInt = inputs::new_random_permutation<int>(n, rng);
	std::sort(pInt, pInt + n);
	ASSERT_TRUE(is_one_up_to_n(pInt, pInt+n));
	delete[] pInt;
}

TEST(inputs, testSortUpAndDown) {
	std::vector<float> v (20) ;
	std::vector<int> l {5,4,2,2,7} ;
	inputs::fill_with_up_and_down_runs(v.begin(), v.end(), l, 1, rng);
	auto a = v.begin();
	ASSERT_TRUE(std::is_sorted(a,a+5));
	ASSERT_TRUE(std::is_sorted(a+5,a+5+4, std::greater<>()));
	ASSERT_TRUE(std::is_sorted(a+9,a+9+2));
	ASSERT_TRUE(std::is_sorted(a+11,a+11+2, std::greater<>()));
	ASSERT_TRUE(std::is_sorted(a+13,a+20));
}

TEST(inputs, testTimsortDragLens) {
	std::vector<int> lens {};
	inputs::compute_timsort_drag_run_lengths(
			[&](int x)->void{lens.push_back(x);}, 16);

	std::vector<int> RTim16 {2, 1, 1, 3, 1, 3, 2, 2, 1};
	ASSERT_EQ(lens, RTim16);
}

TEST(inputs, testTimsortDragGenerator) {
	inputs::timsort_drag_generator<int> td (2);
	int *a = td.newInstance(32, rng);
	ASSERT_TRUE(std::is_sorted(a,a+4));
	ASSERT_TRUE(std::is_sorted(a+4,a+4+2, std::greater<>()));
	ASSERT_TRUE(std::is_sorted(a+6,a+6+2));
	ASSERT_TRUE(std::is_sorted(a+8,a+8+6, std::greater<>()));
	ASSERT_TRUE(std::is_sorted(a+14,a+14+2));
	ASSERT_TRUE(std::is_sorted(a+16,a+16+6, std::greater<>()));
	ASSERT_TRUE(std::is_sorted(a+22,a+22+4));
	ASSERT_TRUE(std::is_sorted(a+26,a+26+4, std::greater<>()));
	ASSERT_TRUE(std::is_sorted(a+30,a+30+2));
}












TEST(harness, testHarness) {
	algorithms::std_sort<vec_iter> ss;
	ASSERT_TRUE(harness_sorter(ss));
}

TEST(harness, harness4wayBasicMerge) {
    ASSERT_TRUE((harness_4way_merge<algorithms::FOR_NUMERIC_DATA_PLAIN_MIN,4>()));
    ASSERT_TRUE((harness_4way_merge<algorithms::FOR_NUMERIC_DATA_PLAIN_MIN,4>(true)));
}

TEST(harness, harness4wayGeneralNoSentinel) {
    ASSERT_TRUE((harness_4way_merge<algorithms::GENERAL_NO_SENTINELS,1>()));
    ASSERT_TRUE((harness_4way_merge<algorithms::GENERAL_NO_SENTINELS,1>(true)));
}

TEST(harness, harness4wayStagesMerge) {
    ASSERT_TRUE((harness_4way_merge<algorithms::GENERAL_BY_STAGES,1>()));
    ASSERT_TRUE((harness_4way_merge<algorithms::GENERAL_BY_STAGES,1>(true)));
}

TEST(harness, harness4wayStagesSplitMerge) {
    ASSERT_TRUE((harness_4way_merge<algorithms::GENERAL_BY_STAGES_SPLIT,1>()));
    ASSERT_TRUE((harness_4way_merge<algorithms::GENERAL_BY_STAGES_SPLIT,1>(true)));
}

TEST(harness, harness4wayWillemMerge) {
    ASSERT_TRUE((harness_4way_merge<algorithms::WILLEM,4>()));
    ASSERT_TRUE((harness_4way_merge<algorithms::WILLEM,4>(true)));
}

TEST(harness, harness4wayNumericData) {
    ASSERT_TRUE((harness_4way_merge<algorithms::FOR_NUMERIC_DATA,4>()));
    ASSERT_TRUE((harness_4way_merge<algorithms::FOR_NUMERIC_DATA,4>(true)));
}

TEST(harness, harness4wayGeneralIndices) {
    ASSERT_TRUE((harness_4way_merge<algorithms::GENERAL_INDICES,1>()));
    ASSERT_TRUE((harness_4way_merge<algorithms::GENERAL_INDICES,1>(true)));
}

TEST(harness, harness4wayWillemIndices) {
    ASSERT_TRUE((harness_4way_merge<algorithms::WILLEM_WITH_INDICES,4>()));
    ASSERT_TRUE((harness_4way_merge<algorithms::WILLEM_WITH_INDICES,3>(true)));
}

TEST(harness, harness4wayWillemTuned) {
    ASSERT_TRUE((harness_4way_merge<algorithms::WILLEM_TUNED,4>()));
    ASSERT_TRUE((harness_4way_merge<algorithms::WILLEM_TUNED,3>(true)));
}

TEST(harness, harnessTopDownMergesort) {
	algorithms::top_down_mergesort<vec_iter , 1, false> tdmp;
	ASSERT_TRUE(harness_sorter(tdmp));
	algorithms::top_down_mergesort<vec_iter , 1, true> tdmp2;
	ASSERT_TRUE(harness_sorter(tdmp2));
	algorithms::top_down_mergesort<vec_iter> tdmp3;
	ASSERT_TRUE(harness_sorter(tdmp3));
}

TEST(harness, harnessBottonUpMergesort) {
	algorithms::bottom_up_mergesort<vec_iter, 1, false> basic;
	ASSERT_TRUE(harness_sorter(basic));
	algorithms::bottom_up_mergesort<vec_iter,7, false> withMinRunLen;
	ASSERT_TRUE(harness_sorter(withMinRunLen));
	algorithms::bottom_up_mergesort<vec_iter,1, true> withCheck;
	ASSERT_TRUE(harness_sorter(withCheck));
}

TEST(harness, harnessPeeksort) {
	algorithms::peeksort<vec_iter, 1, false> basic;
//	checked_vector<int> v { std::vector<int> {6, 4, 3, 1, 2, 5} };
//	basic(v.begin(), v.end());
	ASSERT_TRUE(harness_sorter(basic));
	algorithms::peeksort<vec_iter, 8, false> basic8;
	ASSERT_TRUE(harness_sorter(basic8));
	algorithms::peeksort<vec_iter, 1, true> basicInc;
	ASSERT_TRUE(harness_sorter(basicInc));
}


TEST(harness, harnessPowersort) {
    algorithms::powersort<vec_iter, 1,  algorithms::COPY_BOTH_WITH_SENTINELS, false, algorithms::TRIVIAL, false> sentinel {};
    ASSERT_TRUE(harness_sorter(sentinel));
    algorithms::powersort<vec_iter, 1,  algorithms::UNSTABLE_BITONIC_MERGE, false, algorithms::TRIVIAL, false> paper {};
    ASSERT_TRUE(harness_sorter(paper));
	algorithms::powersort<vec_iter> def;
	ASSERT_TRUE(harness_sorter(def));
	algorithms::powersort<vec_iter, 1, algorithms::UNSTABLE_BITONIC_MERGE, false, algorithms::TRIVIAL, true> basic {};
	ASSERT_TRUE(harness_sorter(basic));
//	checked_vector<int> v { std::vector<int> {6, 4, 3, 1, 2, 5} };
//	basic(v.begin(), v.end());
	algorithms::powersort<vec_iter, 8, algorithms::UNSTABLE_BITONIC_MERGE, false, algorithms::MOST_SIGNIFICANT_SET_BIT> msb {};
	ASSERT_TRUE(harness_sorter(msb));
	algorithms::powersort<vec_iter, 1, algorithms::UNSTABLE_BITONIC_MERGE, true, algorithms::BITWISE_LOOP> inc {};
	ASSERT_TRUE(harness_sorter(inc));

}

TEST(harness, harnessPowersort4Way) {
    algorithms::powersort_4way<vec_iter, 1, algorithms::GENERAL_BY_STAGES_SPLIT, false, algorithms::MOST_SIGNIFICANT_SET_BIT4, true> checkFirst {};
    ASSERT_TRUE(harness_sorter(checkFirst));
    algorithms::powersort_4way<vec_iter, 1, algorithms::GENERAL_BY_STAGES_SPLIT> stages2 {};
    ASSERT_TRUE(harness_sorter(stages2));
    algorithms::powersort_4way<vec_iter, 1, algorithms::GENERAL_BY_STAGES> stages {};
    ASSERT_TRUE(harness_sorter(stages));
    algorithms::powersort_4way<vec_iter, 1, algorithms::GENERAL_NO_SENTINELS> nosentinel {};
    ASSERT_TRUE(harness_sorter(nosentinel));
    algorithms::powersort_4way<vec_iter, 1, algorithms::FOR_NUMERIC_DATA> paper {};
    ASSERT_TRUE(harness_sorter(paper));
    algorithms::powersort_4way<vec_iter, 1, algorithms::WILLEM> willem {};
    ASSERT_TRUE(harness_sorter(willem));
    algorithms::powersort_4way<vec_iter, 1, algorithms::WILLEM_WITH_INDICES> willem_a {};
    ASSERT_TRUE(harness_sorter(willem_a));
    algorithms::powersort_4way<vec_iter, 1, algorithms::WILLEM, false, algorithms::TRIVIAL4> triv {};
    ASSERT_TRUE(harness_sorter(triv));
    algorithms::powersort_4way<vec_iter, 1, algorithms::WILLEM, false, algorithms::BITWISE_LOOP4> bitwisePower {};
    ASSERT_TRUE(harness_sorter(bitwisePower));
    algorithms::powersort_4way<vec_iter, 1, algorithms::WILLEM, false, algorithms::DIVISION_LOOP4> divisionPower {};
    ASSERT_TRUE(harness_sorter(divisionPower));
    algorithms::powersort_4way<vec_iter, 1, algorithms::WILLEM, false, algorithms::MOST_SIGNIFICANT_SET_BIT4> msbsPower {};
    ASSERT_TRUE(harness_sorter(msbsPower));
}

TEST(harness, harnessPowersort4WayWillem) {
    algorithms::powersort_4way<vec_iter, 1, algorithms::WILLEM_VALUES> inMyP4 {};
    ASSERT_TRUE(harness_sorter(inMyP4));
}

TEST(harness, harnessTimsort) {
	algorithms::timsort<vec_iter> tim;
	ASSERT_TRUE(harness_sorter(tim));
}


TEST(harness, harnessTrotsort) {
	algorithms::trotsort<vec_iter> tim;
	ASSERT_TRUE(harness_sorter(tim));
	algorithms::trotsort<vec_iter,true> timBin;
	ASSERT_TRUE(harness_sorter(timBin));
}





TEST(harness, harnessQuicksort) {
	algorithms::quicksort<vec_iter, 1, 1000000000, false> basic;
	for (int n = 10; n>0; n=0) {
//	for (int n = 1; n < 256; ++n) {
		auto *a = inputs::new_random_permutation<int>(n, rng);
		checked_vector<int> v{std::vector<int> {a, a + n}};
		basic(v.begin(), v.end());
		ASSERT_TRUE(std::is_sorted(v.begin(), v.end()));
		delete[] a;
//		std::cout << "mergecosts = " << algorithms::totalMergeCosts << std::endl;
	}

	ASSERT_TRUE(harness_sorter(basic));
	algorithms::quicksort<vec_iter,7, 20, false> withMinRunLen;
	ASSERT_TRUE(harness_sorter(withMinRunLen));
	algorithms::quicksort<vec_iter,1, 1000000000, true> withCheck;
	ASSERT_TRUE(harness_sorter(withCheck));
}


TEST(basics, log2Exmaples) {
    ASSERT_EQ(0, algorithms::floor_log2(1u));
    ASSERT_EQ(3, algorithms::floor_log2(8u));
    ASSERT_EQ(5, algorithms::floor_log2(32u));
    ASSERT_EQ(0, algorithms::floor_log2(1ul));
    ASSERT_EQ(3, algorithms::floor_log2(8ul));
    ASSERT_EQ(5, algorithms::floor_log2(32ul));

    ASSERT_EQ(2, algorithms::floor_log2(5u));
    ASSERT_EQ(2, algorithms::floor_log2(6u));
    ASSERT_EQ(2, algorithms::floor_log2(7u));
    ASSERT_EQ(2, algorithms::floor_log2(5ul));

    ASSERT_EQ(31, algorithms::floor_log2(1u << 31));
    ASSERT_EQ(63, algorithms::floor_log2(1ul << 63));
}

TEST(nodePowers, nodePowersExamples) {
	ASSERT_EQ(algorithms::node_power_trivial(1, 100 + 1, 10, 20, 25 + 1), 4);
	ASSERT_EQ(algorithms::node_power_trivial(0, 21 + 1, 8, 12, 13 + 1), 1);
	ASSERT_EQ(algorithms::node_power_trivial(0, 21 + 1, 19, 20, 20 + 1), 5);
	ASSERT_EQ(algorithms::node_power_trivial(0, 100 * 1000 * 1000 + 1, 55555555, 55555666, 55556666 + 1), 16);

	ASSERT_EQ(algorithms::node_power_bitwise(1, 100 + 1, 10, 20, 25 + 1), 4);
	ASSERT_EQ(algorithms::node_power_bitwise(0, 21 + 1, 8, 12, 13 + 1), 1);
	ASSERT_EQ(algorithms::node_power_bitwise(0, 21 + 1, 19, 20, 20 + 1), 5);
	ASSERT_EQ(algorithms::node_power_bitwise(0, 100 * 1000 * 1000 + 1, 55555555, 55555666, 55556666 + 1), 16);

	ASSERT_EQ(algorithms::node_power_clz(1, 100 + 1, 10, 20, 25 + 1), 4);
	ASSERT_EQ(algorithms::node_power_clz(0, 21 + 1, 8, 12, 13 + 1), 1);
	ASSERT_EQ(algorithms::node_power_clz(0, 21 + 1, 19, 20, 20 + 1), 5);
	ASSERT_EQ(algorithms::node_power_clz(0, 100 * 1000 * 1000 + 1, 55555555, 55555666, 55556666 + 1), 16);

	ASSERT_EQ(algorithms::node_power_div(1, 100 + 1, 10, 20, 25 + 1), 4);
	ASSERT_EQ(algorithms::node_power_div(0, 21 + 1, 8, 12, 13 + 1), 1);
	ASSERT_EQ(algorithms::node_power_div(0, 21 + 1, 19, 20, 20 + 1), 5);
	ASSERT_EQ(algorithms::node_power_div(0, 100 * 1000 * 1000 + 1, 55555555, 55555666, 55556666 + 1), 16);

}


TEST(nodePowers, nodePowers4Examples) {
	ASSERT_EQ(algorithms::node_power4_trivial(1, 100 + 1, 10, 20, 25 + 1), 2);
	ASSERT_EQ(algorithms::node_power4_trivial(0, 21 + 1, 8, 12, 13 + 1), 1);
	ASSERT_EQ(algorithms::node_power4_trivial(0, 21 + 1, 19, 20, 20 + 1), 3);
	ASSERT_EQ(algorithms::node_power4_trivial(0, 100 * 1000 * 1000 + 1, 55555555, 55555666, 55556666 + 1), 8);

	ASSERT_EQ(algorithms::node_power4_bitwise(1, 100 + 1, 10, 20, 25 + 1), 2);
	ASSERT_EQ(algorithms::node_power4_bitwise(0, 21 + 1, 8, 12, 13 + 1), 1);
	ASSERT_EQ(algorithms::node_power4_bitwise(0, 21 + 1, 19, 20, 20 + 1), 3);
	ASSERT_EQ(algorithms::node_power4_bitwise(0, 100 * 1000 * 1000 + 1, 55555555, 55555666, 55556666 + 1), 8);

	ASSERT_EQ(algorithms::node_power4_clz(1, 100 + 1, 10, 20, 25 + 1), 2);
	ASSERT_EQ(algorithms::node_power4_clz(0, 21 + 1, 8, 12, 13 + 1), 1);
	ASSERT_EQ(algorithms::node_power4_clz(0, 21 + 1, 19, 20, 20 + 1), 3);
	ASSERT_EQ(algorithms::node_power4_clz(0, 100 * 1000 * 1000 + 1, 55555555, 55555666, 55556666 + 1), 8);

	ASSERT_EQ(algorithms::node_power4_div(1, 100 + 1, 10, 20, 25 + 1), 2);
	ASSERT_EQ(algorithms::node_power4_div(0, 21 + 1, 8, 12, 13 + 1), 1);
	ASSERT_EQ(algorithms::node_power4_div(0, 21 + 1, 19, 20, 20 + 1), 3);
	ASSERT_EQ(algorithms::node_power4_div(0, 100 * 1000 * 1000 + 1, 55555555, 55555666, 55556666 + 1), 8);

}





TEST(welford, testVariance) {
    std::vector<double> v {1, 2, 3, 4, 5, 6, 7, 8, 9, 10,1, 2, 3, 4, 5, 6, 7, 8, 9, 10,1, 2, 3, 4, 5, 6, 7, 8, 9, 10,1, 2, 3, 4, 5, 6, 7, 8, 9, 10,};
    util::welford_variance w;
    for (auto x : v) {
        w.add_sample(x);
    }
    ASSERT_DOUBLE_EQ(w.mean(), 5.5);
    ASSERT_DOUBLE_EQ(w.variance(), 8.4615384615384615385);
    ASSERT_DOUBLE_EQ(w.stdev(), 2.9088723694136980025);
    ASSERT_DOUBLE_EQ(w.stderror(), 0.45993310550389992389);
    ASSERT_EQ(w.meanSignificantDigits(), "5.*");

    std::vector<double> v2 {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 9};
    util::welford_variance w2;
    for (auto x : v2) {
        w2.add_sample(x);
    }
    ASSERT_DOUBLE_EQ(w2.mean(), 9.9523809523809523810);
    ASSERT_DOUBLE_EQ(w2.variance(), 0.047619047619047619048);
    ASSERT_DOUBLE_EQ(w2.stdev(), 0.21821789023599238127);
    ASSERT_DOUBLE_EQ(w2.stderror(), 0.047619047619047619048);
    ASSERT_EQ(w2.meanSignificantDigits(), "9.9*");
}
