/** @author Sebastian Wild (wild@liverpool.ac.uk) */


#ifndef MERGESORTS_DATATYPES_H
#define MERGESORTS_DATATYPES_H

#include <cassert>
#include <algorithm>
#include <random>
#include <iostream>

namespace data {

    long long volatile totalComparisons = 0ll;

    class comp_counter {
        int _value;
    public:
        comp_counter(int _value = 0) : _value(_value) {} // convert from int

        comp_counter(comp_counter const &other) = default;
        comp_counter(comp_counter &&other) = default;
        comp_counter &operator=(comp_counter const &rhs) = default;

        bool operator<(const comp_counter &rhs) const { // comparison operator
            ++totalComparisons;
            return _value < rhs._value;
        }
        bool operator==(const comp_counter &rhs) const { return _value == rhs._value; }
        bool operator!=(const comp_counter &rhs) const { return !(rhs == *this); }
        bool operator>(const comp_counter &rhs) const { return rhs < *this; }
        bool operator<=(const comp_counter &rhs) const { return !(rhs < *this); }
        bool operator>=(const comp_counter &rhs) const { return !(*this < rhs); }

        friend std::ostream &operator<<(std::ostream &os, const comp_counter &counter) { // print
            os << counter._value; return os;
        }

        comp_counter &operator+=(comp_counter const &rhs) { // increment operator
            _value += rhs._value; return *this;
        }
    };

    enum blob_comparison_function {
        FIRST_ENTRY,
        LEXICOGRAPHIC,
        HASH_OF_ALL
    };

    const int PRIMES_ABOVE_TWO_TO_TWENTY[] {1073741827,1073741831,1073741833,1073741839,1073741843,1073741857,1073741891,1073741909,1073741939,1073741953,1073741969,1073741971,1073741987,1073741993,1073742037,1073742053,1073742073,1073742077,1073742091,1073742113,1073742169,1073742203,1073742209,1073742223,1073742233,1073742259,1073742277,1073742289,1073742343,1073742353,1073742361,1073742391,1073742403,1073742463,1073742493,1073742517,1073742583,1073742623,1073742653,1073742667,1073742671,1073742673,1073742707,1073742713,1073742721,1073742731,1073742767,1073742773,1073742811,1073742851,1073742853,1073742881,1073742889,1073742913,1073742931,1073742937,1073742959,1073742983,1073743007,1073743037,1073743049,1073743051,1073743079,1073743091};

    /** an int array of given size */
    template<int size, typename Int = int, blob_comparison_function comparisonFunction = FIRST_ENTRY> class blob {
    private:
        static_assert(size <= 64, "have not provisioned blob for more than size 64");
    public:
        Int a[size];

        blob(int v = 0) :  a {} { // conversion from int
            for (int i = 0; i < size; ++i) a[i] = v % PRIMES_ABOVE_TWO_TO_TWENTY[i];
        }
        bool operator<(const blob<size, Int, comparisonFunction> &rhs) const {
            switch (comparisonFunction) {
                case blob_comparison_function::FIRST_ENTRY:
                    return a[0] < rhs.a[0];
                case blob_comparison_function::LEXICOGRAPHIC:
                    for (int i = 0; i < size; ++i) {
                        if (a[i] < rhs.a[i]) return true;
                        if (a[i] > rhs.a[i]) return false;
                    }
                    return false;
                case blob_comparison_function::HASH_OF_ALL:
                    return hash() < rhs.hash();
            }
        }

        int hash() const {
            // compute some linear congruential hash
            const auto p = 2147483659; // large prime
            const auto a = 3952532, b = 23895293; // "random" numbers
            auto h = 0;
            for (int i = 0; i < size; ++i) h += (a * this->a[i] + b) % p;
            return h;
        }

        bool operator>(const blob<size, Int, comparisonFunction> &rhs) const { return rhs < *this; }
        bool operator<=(const blob<size, Int, comparisonFunction> &rhs) const { return !(rhs < *this); }
        bool operator>=(const blob<size, Int, comparisonFunction> &rhs) const { return !(*this < rhs); }
//        bool operator==(const comp_counter &rhs) const { return _value == rhs._value; }
//        bool operator!=(const comp_counter &rhs) const { return !(rhs == *this); }
        friend std::ostream &operator<<(std::ostream &os, const blob<size, Int, comparisonFunction> &blob1) {
            os << "b[";
            for (int i = 0; i < size; ++i) os << blob1.a[i] << ",";
            return os << "]";
        }

        blob<size, Int, comparisonFunction> &operator+=(blob<size, Int, comparisonFunction> const &rhs) { // increment operator
            for (int i = 0; i < size; ++i)
                a[i] = (a[i] + rhs.a[i] % PRIMES_ABOVE_TWO_TO_TWENTY[i]);
            return *this;
        }
    };

}
typedef data::blob<8> blob32b;
typedef data::blob<32> blob128b;
typedef data::blob<8, int, data::HASH_OF_ALL> blob8b_hash;
typedef data::blob<2, long, data::FIRST_ENTRY> blob_long_and_pointer;

using data::comp_counter;

namespace std {
    // specialize std::numeric-limits for comp_counter
    template<>
    struct numeric_limits<comp_counter> {
        static const bool is_specialized = true;
        static const comp_counter min() noexcept { return comp_counter(numeric_limits<int>::min()); }
        static const comp_counter max() noexcept { return comp_counter(numeric_limits<int>::max()); }
        static const comp_counter lowest() noexcept { return comp_counter(numeric_limits<int>::lowest()); }
        static constexpr int digits = numeric_limits<int>::digits;
        static constexpr int digits10 = numeric_limits<int>::digits10;
        static constexpr int max_digits10 = numeric_limits<int>::max_digits10;
        static constexpr bool is_signed = numeric_limits<int>::is_signed;
        static constexpr bool is_integer = numeric_limits<int>::is_integer;
        static constexpr bool is_exact = numeric_limits<int>::is_exact;
        static constexpr int radix = numeric_limits<int>::radix;
        static constexpr int epsilon() noexcept { return numeric_limits<int>::epsilon(); }
        static constexpr int round_error() noexcept { return numeric_limits<int>::round_error(); }
        static constexpr int min_exponent = numeric_limits<int>::min_exponent;
        static constexpr int min_exponent10 = numeric_limits<int>::min_exponent10;
        static constexpr int max_exponent = numeric_limits<int>::max_exponent;
        static constexpr int max_exponent10 = numeric_limits<int>::max_exponent10;
        static constexpr bool has_infinity = numeric_limits<int>::has_infinity;
        static constexpr bool has_quiet_NaN = numeric_limits<int>::has_quiet_NaN;
        static constexpr bool has_signaling_NaN = numeric_limits<int>::has_signaling_NaN;
        static constexpr float_denorm_style has_denorm = numeric_limits<int>::has_denorm;
        static constexpr bool has_denorm_loss = numeric_limits<int>::has_denorm_loss;
        static const comp_counter infinity() noexcept { return comp_counter(numeric_limits<int>::infinity()); }
        static const comp_counter quiet_NaN() noexcept { return comp_counter(numeric_limits<int>::quiet_NaN()); }
        static const comp_counter signaling_NaN() noexcept {
            return comp_counter(numeric_limits<int>::signaling_NaN());
        }
        static const comp_counter denorm_min() noexcept { return comp_counter(numeric_limits<int>::denorm_min()); }
        static constexpr bool is_iec559 = numeric_limits<int>::is_iec559;
        static constexpr bool is_bounded = numeric_limits<int>::is_bounded;
        static constexpr bool is_modulo = numeric_limits<int>::is_modulo;
        static constexpr bool traps = numeric_limits<int>::traps;
        static constexpr bool tinyness_before = numeric_limits<int>::tinyness_before;
        static constexpr float_round_style round_style = numeric_limits<int>::round_style;
    };

    // specialize std::numeric-limits for blob
    template<int size, typename Int, data::blob_comparison_function comparisonFunction>
    struct numeric_limits<data::blob<size, Int, comparisonFunction>> {
        static const bool is_specialized = comparisonFunction != data::blob_comparison_function::HASH_OF_ALL;
        static const data::blob<size, Int, comparisonFunction> min() noexcept {
            data::blob<size, Int, comparisonFunction> res = data::blob<size, Int, comparisonFunction>();
            for (int i = 0; i < size; ++i) res.a[i] = 1-data::PRIMES_ABOVE_TWO_TO_TWENTY[i];
            return res; 
        }
        static const data::blob<size, Int, comparisonFunction> max() noexcept {
            data::blob<size, Int, comparisonFunction> res = data::blob<size, Int, comparisonFunction>();
            for (int i = 0; i < size; ++i) res.a[i] = data::PRIMES_ABOVE_TWO_TO_TWENTY[i]-1;
            return res; 
        }
        static const data::blob<size, Int, comparisonFunction> lowest() noexcept { return data::blob<size, Int, comparisonFunction>(numeric_limits<Int>::lowest()); }
        static constexpr int digits = numeric_limits<Int>::digits;
        static constexpr int digits10 = numeric_limits<Int>::digits10;
        static constexpr int max_digits10 = numeric_limits<Int>::max_digits10;
        static constexpr bool is_signed = numeric_limits<Int>::is_signed;
        static constexpr bool is_integer = numeric_limits<Int>::is_integer;
        static constexpr bool is_exact = numeric_limits<Int>::is_exact;
        static constexpr int radix = numeric_limits<Int>::radix;
        static constexpr int epsilon() noexcept { return numeric_limits<Int>::epsilon(); }
        static constexpr int round_error() noexcept { return numeric_limits<Int>::round_error(); }
        static constexpr int min_exponent = numeric_limits<Int>::min_exponent;
        static constexpr int min_exponent10 = numeric_limits<Int>::min_exponent10;
        static constexpr int max_exponent = numeric_limits<Int>::max_exponent;
        static constexpr int max_exponent10 = numeric_limits<Int>::max_exponent10;
        static constexpr bool has_infinity =  comparisonFunction != data::blob_comparison_function::HASH_OF_ALL;
        static constexpr bool has_quiet_NaN = numeric_limits<Int>::has_quiet_NaN;
        static constexpr bool has_signaling_NaN = numeric_limits<Int>::has_signaling_NaN;
        static constexpr float_denorm_style has_denorm = numeric_limits<Int>::has_denorm;
        static constexpr bool has_denorm_loss = numeric_limits<Int>::has_denorm_loss;
        static const data::blob<size, Int, comparisonFunction> infinity() noexcept {
            data::blob<size, Int, comparisonFunction> res = data::blob<size, Int, comparisonFunction>(0);
            res.a[0] = numeric_limits<Int>::max();
            return res; }
        static const data::blob<size, Int, comparisonFunction> quiet_NaN() noexcept { return data::blob<size, Int, comparisonFunction>(numeric_limits<Int>::quiet_NaN()); }
        static const data::blob<size, Int, comparisonFunction> signaling_NaN() noexcept {
            return data::blob<size, Int, comparisonFunction>(numeric_limits<Int>::signaling_NaN());
        }
        static const data::blob<size, Int, comparisonFunction> denorm_min() noexcept { return data::blob<size, Int, comparisonFunction>(numeric_limits<Int>::denorm_min()); }
        static constexpr bool is_iec559 = numeric_limits<Int>::is_iec559;
        static constexpr bool is_bounded = numeric_limits<Int>::is_bounded;
        static constexpr bool is_modulo = numeric_limits<Int>::is_modulo;
        static constexpr bool traps = numeric_limits<Int>::traps;
        static constexpr bool tinyness_before = numeric_limits<Int>::tinyness_before;
        static constexpr float_round_style round_style = numeric_limits<Int>::round_style;
    };
}


#endif //MERGESORTS_DATATYPES_H
