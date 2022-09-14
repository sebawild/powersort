//
// Created by seb on 5/22/18.
//

#ifndef MERGESORTS_SORTER_HARNESS_H
#define MERGESORTS_SORTER_HARNESS_H

#include <random>
#include <inputs.h>
#include <vector>
#include <iostream>
#include <include/gtest/gtest.h>
#include "../algorithms.h"
#include "checked_vector.h"
#include "merging_multiway.h"
#include "merging_3way.h"

template<typename Iter>
bool is_one_up_to_n(Iter start, Iter end)
{
	int x = 1;
	for (Iter i = start; i < end; ++i)
		if (*i != x++) return false;
	return true;
}

struct not_sorted : public std::exception {
	const char *what() const noexcept override {
		return "result not 1..n";
	}
};


template <algorithms::merging4way_methods mergingMethod, int bufferOversize = 4>
bool harness_4way_merge(bool harness_3way_method_instead = false)
{
    auto k = harness_3way_method_instead ? 3 : 4;
	std::random_device r;
	inputs::RNG rng(r());
	for (int n = 3; n < 1000; n+=1) {
		for (int iter = 0; iter < 10; ++iter) {
			int *a = inputs::new_random_permutation<int>(n, rng);
            auto it = inputs::sort_k_random_runs(a, a + n, k, rng);
            checked_vector<int> copy (std::vector<int> (a,a+n));
            checked_vector<int> B (std::vector<int> (n+bufferOversize));
			try {
                if (harness_3way_method_instead)
                    algorithms::merge_3runs<mergingMethod>(copy.begin(),
                                                           copy.begin() + (it[1]-it[0]),
                                                           copy.begin() + (it[2]-it[0]),
                                                           copy.begin() + (it[3]-it[0]),
                                                           B.begin());
                else
                    algorithms::merge_4runs<mergingMethod>(copy.begin(),
                                                           copy.begin() + (it[1]-it[0]),
                                                           copy.begin() + (it[2]-it[0]),
                                                           copy.begin() + (it[3]-it[0]),
                                                           copy.begin() + (it[4]-it[0]),
                                                           B.begin());
				if (!is_one_up_to_n(copy.begin(), copy.end())) {
					throw not_sorted{};
				}
			} catch (std::exception & e) {
				std::cerr << "ERROR while merging with " << algorithms::to_string(mergingMethod) << ":\n" << e.what();
				std::cerr << "original input = [";
				for (int* i = a; i < a + n; ++i) std::cerr << *i << ", ";
				std::cerr << "]" << std::endl;
				std::cerr << "'merged' output = [";
				for (int i : copy) std::cerr << i << ", ";
				std::cerr << "]" << std::endl;
                std::cerr << "run lengths = [" << it[1] - it[0] << ", " << it[2] - it[1] << ", " << it[3] - it[2] << ", " << it[4] - it[3] << "]" << std::endl;
				return false;
			}
			delete[] a;
		}
	}
	for (int n = 3; n < 1000; n+=1) {
		for (int iter = 0; iter < 10; ++iter) {
            std::vector<int> a(n);
            inputs::fill_with_iid_uary(a.begin(), a.end(), 2, rng);
            auto it = inputs::sort_k_random_runs(a.begin(), a.end(), k, rng);
            checked_vector<int> copy (std::vector<int> (a.begin(), a.end()));
            checked_vector<int> B (std::vector<int> (n+bufferOversize));
			try {
                if (harness_3way_method_instead)
                    algorithms::merge_3runs<mergingMethod>(copy.begin(),
                                                           copy.begin() + (it[1]-it[0]),
                                                           copy.begin() + (it[2]-it[0]),
                                                           copy.begin() + (it[3]-it[0]),
                                                           B.begin());
                else
                    algorithms::merge_4runs<mergingMethod>(copy.begin(),
                                                           copy.begin() + (it[1]-it[0]),
                                                           copy.begin() + (it[2]-it[0]),
                                                           copy.begin() + (it[3]-it[0]),
                                                           copy.begin() + (it[4]-it[0]),
                                                           B.begin());
				if (!is_sorted(copy.begin(), copy.end())) {
					throw not_sorted{};
				}
			} catch (std::exception & e) {
				std::cerr << "ERROR while merging with " << algorithms::to_string(mergingMethod) << ":\n" << e.what();
                // print e's stacktrace

				std::cerr << "original input = [";
                for (int i : a) std::cerr << i << ", ";
				std::cerr << "]" << std::endl;
				std::cerr << "'merged' output = [";
				for (int i : copy) std::cerr << i << ", ";
				std::cerr << "]" << std::endl;
				return false;
			}
		}
	}
    int n = 1000000;
    for (int iter = 0; iter < 10; ++iter) {
        int *a = inputs::new_random_permutation<int>(n, rng);
        auto it = inputs::sort_k_random_runs(a, a + n, k, rng);
        checked_vector<int> copy (std::vector<int> (a,a+n));
        checked_vector<int> B (std::vector<int> (n+bufferOversize));
        try {
            if (harness_3way_method_instead)
                algorithms::merge_3runs<mergingMethod>(copy.begin(),
                                                       copy.begin() + (it[1]-it[0]),
                                                       copy.begin() + (it[2]-it[0]),
                                                       copy.begin() + (it[3]-it[0]),
                                                       B.begin());
            else
                algorithms::merge_4runs<mergingMethod>(copy.begin(),
                                                       copy.begin() + (it[1]-it[0]),
                                                       copy.begin() + (it[2]-it[0]),
                                                       copy.begin() + (it[3]-it[0]),
                                                       copy.begin() + (it[4]-it[0]),
                                                       B.begin());
            if (!is_one_up_to_n(copy.begin(), copy.end())) {
                throw not_sorted{};
            }
        } catch (std::exception & e) {
            std::cerr << "ERROR while merging with " << algorithms::to_string(mergingMethod) << ":\n" << e.what();
            std::cerr << "original input = [";
            for (int* i = a; i < a + n; ++i) std::cerr << *i << ", ";
            std::cerr << "]" << std::endl;
            std::cerr << "'merged' output = [";
            for (int i : copy) std::cerr << i << ", ";
            std::cerr << "]" << std::endl;
            return false;
        }
        delete[] a;
		}

    return true;
}


template<typename Sorter>
bool harness_sorter(Sorter & sorter)
{
	std::random_device r;
	inputs::RNG rng(r());
	
	for (int n = 3; n < 1000; n+=1) {
		for (int iter = 0; iter < 10; ++iter) {
			int *a = inputs::new_random_permutation<int>(n, rng);
			checked_vector<int> copy (std::vector<int> (a,a+n));
			try {
				sorter(copy.begin(), copy.end());
				if (!is_one_up_to_n(copy.begin(), copy.end())) {
					throw not_sorted{};
				}
			} catch (std::exception & e) {
				std::cerr << "ERROR while sorting with " << sorter.name() << ":\n" << e.what();
				std::cerr << "original input = [";
				for (int* i = a; i < a + n; ++i) std::cerr << *i << ", ";
				std::cerr << "]" << std::endl;
				std::cerr << "'sorted' output = [";
				for (int i : copy) std::cerr << i << ", ";
				std::cerr << "]" << std::endl;
				return false;
			}
			delete[] a;
		}
	}

	for (int n = 3; n < 1000; n+=1) {
		for (int iter = 0; iter < 10; ++iter) {
			std::vector<int> a(n);
			inputs::fill_with_iid_uary(a.begin(), a.end(), 2, rng);
			checked_vector<int> copy {a};
			try {
				sorter(copy.begin(), copy.end());
				if (!std::is_sorted(copy.begin(), copy.end())) {
					throw not_sorted{};
				}
			} catch (std::exception & e) {
				std::cerr << "ERROR while sorting with " << sorter.name() << ":\n" << e.what();
				std::cerr << "original input = [";
				for (int i : a) std::cerr << i << ", ";
				std::cerr << "]" << std::endl;
				std::cerr << "'sorted' output = [";
				for (int i : copy) std::cerr << i << ", ";
				std::cerr << "]" << std::endl;
				return false;
			}
		}
	}

	{
        int sizes[] = {1000000, 24 * 1 << 10};
		for (int n : sizes) {
            for (int iter = 0; iter < 10; ++iter) {
                int *a = inputs::new_random_permutation<int>(n, rng);
                checked_vector<int> copy(std::vector<int>(a, a + n));
                try {
                    sorter(copy.begin(), copy.end());
                    if (!is_one_up_to_n(copy.begin(), copy.end())) {
                        throw not_sorted{};
                    }
                } catch (std::exception &e) {
                    std::cerr << "ERROR while sorting with " << sorter.name() << ":\n" << e.what();
                    std::cerr << "original input = [";
                    for (int *i = a; i < a + n; ++i) std::cerr << *i << ", ";
                    std::cerr << "]" << std::endl;
                    std::cerr << "'sorted' output = [";
                    for (int i: copy) std::cerr << i << ", ";
                    std::cerr << "]" << std::endl;
                    return false;
                }
                delete[] a;
            }
        }
	}
	
	{
		int n = 100000;
		auto ig = inputs::random_runs_generator<int>(500);
		for (int iter = 0; iter < 10; ++iter) {
			int *a = ig.newInstance(n, rng);
			checked_vector<int> copy (std::vector<int> (a,a+n));
			try {
				sorter(copy.begin(), copy.end());
				if (!is_one_up_to_n(copy.begin(), copy.end())) {
					throw not_sorted{};
				}
			} catch (std::exception & e) {
				std::cerr << "ERROR while sorting with " << sorter.name() << ":\n" << e.what();
				std::cerr << "original input = [";
				for (int* i = a; i < a + n; ++i) std::cerr << *i << ", ";
				std::cerr << "]" << std::endl;
				std::cerr << "'sorted' output = [";
				for (int i : copy) std::cerr << i << ", ";
				std::cerr << "]" << std::endl;
				return false;
			}
			delete[] a;
		}
	}



	return true;
}



#endif //MERGESORTS_SORTER_HARNESS_H
