#include <iostream>
#include <vector>
#include <memory>
#include <iomanip>
#include <fstream>
#include <chrono>

#include "algorithms.h"
#include "inputs.h"
#include "welford.h"
#include "sorts/top_down_mergesort.h"
#include "sorts/bottom_up_mergesort.h"
#include "sorts/peeksort.h"
#include "sorts/powersort.h"
#include "sorts/powersort_4way.h"
#include "sorts/timsort.h"
#include "sorts/trotsort.h"
#include "sorts/quicksort.h"
#include "sorts/merging.h"
#include "sorts/merging_multiway.h"
#include "datatypes.h"

static bool ABORT_IF_RESULT_NOT_SORTED = true;

template<typename Iterator>
std::vector<std::unique_ptr<algorithms::sorter<Iterator>>> contestants() {
	std::vector<std::unique_ptr<algorithms::sorter<Iterator>>> algos;
    algos.push_back(std::make_unique<algorithms::nop<Iterator>>());

#ifndef EXCLUDE_POWERSORTS_WITH_SENTINEL
    algos.push_back(std::make_unique<algorithms::powersort_4way<Iterator,24,algorithms::WILLEM_TUNED>>());
#endif // EXCLUDE_POWERSORTS_WITH_SENTINEL
    algos.push_back(std::make_unique<algorithms::powersort_4way<Iterator,24,algorithms::GENERAL_BY_STAGES_SPLIT>>());

#ifndef EXCLUDE_POWERSORTS_WITH_SENTINEL
    algos.push_back(std::make_unique<algorithms::powersort<Iterator,24,algorithms::COPY_BOTH_WITH_SENTINELS>>());
#endif // EXCLUDE_POWERSORTS_WITH_SENTINEL
    algos.push_back(std::make_unique<algorithms::powersort<Iterator,24,algorithms::COPY_BOTH>>());
    algos.push_back(std::make_unique<algorithms::powersort<Iterator,24,algorithms::COPY_SMALLER>>());

	algos.push_back(std::make_unique<algorithms::std_sort<Iterator>>());
	algos.push_back(std::make_unique<algorithms::std_stable_sort<Iterator>>());
	algos.push_back(std::make_unique<algorithms::top_down_mergesort<Iterator, 24, true>>());


	// Further algorithms and variants

#ifndef EXCLUDE_POWERSORTS_WITH_SENTINEL
    algos.push_back(std::make_unique<algorithms::powersort_4way<Iterator,24,algorithms::WILLEM_VALUES>>());
    algos.push_back(std::make_unique<algorithms::powersort_4way<Iterator,24,algorithms::WILLEM_WITH_INDICES>>());
#endif // EXCLUDE_POWERSORTS_WITH_SENTINEL
    algos.push_back(std::make_unique<algorithms::powersort_4way<Iterator,24,algorithms::GENERAL_BY_STAGES>>());


	algos.push_back(std::make_unique<algorithms::peeksort<Iterator, 24>>());

	algos.push_back(std::make_unique<algorithms::quicksort<Iterator, 24, 128, false>>());
	algos.push_back(std::make_unique<algorithms::quicksort<Iterator, 24, 128, true>>());

	algos.push_back(std::make_unique<algorithms::top_down_mergesort<Iterator, 1, true>>());
	algos.push_back(std::make_unique<algorithms::top_down_mergesort<Iterator, 1, false>>());
	algos.push_back(std::make_unique<algorithms::bottom_up_mergesort<Iterator, 24, true>>());

	algos.push_back(std::make_unique<algorithms::timsort<Iterator>>());
	algos.push_back(std::make_unique<algorithms::trotsort<Iterator, false>>());
	algos.push_back(std::make_unique<algorithms::trotsort<Iterator, true>>());
	algos.push_back(std::make_unique<algorithms::nop<Iterator, true>>());

	return algos;

}

template<typename Elem>
void timeSorts(int reps, std::vector<int> sizes, unsigned long seed, inputs::input_generator<Elem> &inputs,
               std::string outFileName, int onlyRunContestant) {
	std::ofstream csv;
	std::string filename;
	{ // Construct filename
		std::ostringstream longFilename;
		std::time_t now = std::time(nullptr);
		std::tm tm = *std::localtime(&now);
		longFilename << outFileName;
		longFilename << std::put_time(&tm, "-%Y-%m-%d_%H-%M-%S");
		longFilename << "-reps" << reps;
		longFilename << "-ns";
		for (int n : sizes) longFilename << "-" << n;
		longFilename << "-seed" << seed;
		longFilename << "-elemT" << typeid(Elem).name();
		longFilename << ".csv";
		filename = longFilename.str();
	}
	csv.open(filename);

	if (typeid(Elem).hash_code() == typeid(data::comp_counter).hash_code()) {
		csv << "algo,ms,n,input,input-num,merge-cost,buffer-cost,comparisons" << std::endl;
		std::cout << "Counting comparisons." << std::endl;
	} else {
		csv << "algo,ms,n,input,input-num,merge-cost" << std::endl;
		std::cout << "Not counting comparisons." << std::endl;
	}
    if (algorithms::COUNT_MERGE_COSTS) std::cout << "Counting merge costs." << std::endl;
	if (!csv.is_open()) {
		std::cout << "Could not open file " << filename << " for writing! Exiting." << std::endl;
		exit(1);
	}


	auto algos = contestants<Elem *>();

	// Dump config
	std::cout << "algos =\n";
	for (int i = 0; i < algos.size(); ++i)
        std::cout << "\t" << i << " : " << algos[i]->name() << "\n";
    std::cout << "reps = " << reps << std::endl;
    std::cout << "sizes = [";
    for (int size : sizes) std::cout << size << " ";
    std::cout << "]" << std::endl;
    std::cout << "inputs = " << inputs << std::endl;
    std::cout << "onlyRunContestant = " << onlyRunContestant << std::endl;
    std::cout << "seed = " << seed << std::endl;
	std::cout << "Writing to " << filename << std::endl;
	std::cout << "Sorting " << typeid(Elem).name() << "s (" << sizeof(Elem) << " byte each)" << std::endl;

	std::cout << "\nRuns with individual timing (skips first run):" << std::endl;

    int algoId = 0;
	for (auto &&algo : algos) {
        if (0 <= onlyRunContestant && onlyRunContestant < algos.size() && algoId++ != onlyRunContestant) continue;
		inputs::RNG rng(seed);
		for (int size : sizes) {
			util::welford_variance samples;
			Elem total = 0;
			Elem *input = inputs.next(size, rng, nullptr);
			for (int r = 0; r < reps; ++r) {
				if (r != 0) input = inputs.next(size, rng, input);
				algorithms::totalMergeCosts = 0;
				algorithms::totalBufferCosts = 0;
				data::totalComparisons = 0;

				auto begin = std::chrono::high_resolution_clock::now();
				algo->sort(input, input + size);
				auto end = std::chrono::high_resolution_clock::now();
				long long int nCmps = data::totalComparisons;
				total += input[size/2];
				if (algo->is_real_sort()) {
					if (!std::is_sorted(input, input + size)) {
						std::cerr << "Input not sorted! " << algo->name() << std::endl;
						exit(3);
					}
				}
				double msDiff = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count() / 1e6;
				if (r != 0) {
					// Skip first iteration, slower because of cold cache
                    // and buffer allocation (latter is reused afterwards).
					samples.add_sample(msDiff);
					csv << algo->name() << "," << msDiff << "," << size << "," << inputs.name() << "," << r << "," << algorithms::totalMergeCosts << "," << algorithms::totalBufferCosts;
					if (typeid(Elem).hash_code() == typeid(comp_counter).hash_code()) {
						csv << "," << nCmps;
					}
					csv << std::endl;
					csv.flush();
				}
			}
			std::cout << "avg-ms=" << samples.meanSignificantDigits() << ",\t algo=" << algo->name() << ", n=" << size << "     (" << total<<")\t" << samples << std::endl;

			delete[] input;
		}

	}

	std::time_t now = std::time(nullptr);
	std::tm tm = *std::localtime(&now);
	csv << "#finished: "<< std::put_time(&tm, "-%Y-%m-%d_%H-%M-%S") << std::endl;
	csv.close();
}


typedef unsigned short int Short;

#ifndef ELEM_T
#define ELEM_T int
#endif

typedef ELEM_T elem_t;



int main(int argc, char **argv) {
    std::cout << std::boolalpha; // format bool as true/false

	if (argc == 1) {
		std::cout << "Usage: mergesorts [reps] [n1,n2,n3] [inputs] [contestants] [seed] [outfile]" << std::endl;
	}

	int reps = 11;
	if (argc >= 2) {
		reps = std::atoi(argv[1]);
	}
	std::vector<int> sizes{10000000};
	if (argc >= 3) {
		sizes.clear();
		std::stringstream ns { argv[2] };
		while (ns.good()) {
			std::string substr;
			std::getline( ns, substr, ',' );
			sizes.push_back( std::stoi(substr) );
		}
	}
	inputs::input_generator<elem_t> *inputs =
			new inputs::random_runs_generator<elem_t>(round(sqrt(sizes[0])));
	if (argc >= 4) {
		std::string ins {argv[3]};
		delete inputs;
		if (ins == "rp" || ins == "random-permutations")
			inputs = new inputs::random_permutations_generator<elem_t> {};
		if (ins.substr(0,4) == "runs") {
            if (ins.substr(5, 5) == "sqrtn") // runs-sqrtn
                inputs = new inputs::random_runs_sqrt_n_generator<elem_t>();
            else
                inputs = new inputs::random_runs_generator<elem_t>(
                        std::stoi(ins.substr(4)));
        }
		if (ins.substr(0,7) == "timdrag")
			inputs = new inputs::timsort_drag_generator<elem_t>(
					std::stoi(ins.substr(7)));

	}
    int onlyRunContestant = -1;
    if (argc >= 5) {
        std::string contestant{argv[4]};
        if (contestant.substr(0,1) == "*") {
            // keep all contestants
            onlyRunContestant = -1;
        } else {
            int i = std::stoi(contestant);
            onlyRunContestant = i;
        }
    }
    unsigned long seed = 439569436534ul;
    if (argc >= 6) {
        seed = std::atol(argv[5]);
    }
	std::string filename("times");
	if (argc >= 7) {
		filename = std::string(argv[6]);
	}
    timeSorts<elem_t>(reps, sizes, seed, *inputs, filename, onlyRunContestant);

	delete inputs;

	return 0;
}
