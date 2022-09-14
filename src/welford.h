//
// Implementation of Welford's streaming algorithm to compute the variance
// Created by seb on 5/19/18.
//

#ifndef MERGESORTS_WELFORD_H
#define MERGESORTS_WELFORD_H

#include <iostream>
#include <cmath>

namespace util {

    /**
     * Simple implementation of Welford's algorithm for
     * online-computation of the variance of a stream.
     *
     * see http://jonisalonen.com/2013/deriving-welfords-method-for-computing-variance/
     *
     * @author Sebastian Wild (wild@liverpool.ac.uk)
     */
    class welford_variance
    {
    private:
        int _nSamples = 0;
        double _mean = 0, _squaredError = 0;

    public:
        void add_sample(double x)
        {
            ++_nSamples;
            double oldMean = _mean;
            _mean += (x - _mean) / _nSamples;
            _squaredError += (x - _mean) * (x - oldMean);
        }

        double mean() const {
            return _mean;
        }

        int nSamples() const {
            return _nSamples;
        }

        double variance() const {
            return _squaredError / (_nSamples - 1);
        }

        double stdev() const {
            return sqrt(variance());
        }

        double stderror() const {
            return stdev() / std::sqrt(_nSamples);
        }

        std::string meanSignificantDigits() const {
            double meanPlusStderr = mean() + stderror();
            double meanMinusStderr = mean() - stderror();
            int precision = round(log10(2*stderror()));
            const std::string m = std::to_string(mean());
            const std::string u = std::to_string(meanPlusStderr);
            const std::string l = std::to_string(meanMinusStderr);
            // find longest common prefix of l, m, u
            int il = 0;
            while (il < l.size() && il < m.size() && l[il] == m[il]) ++il;
            int iu = 0;
            while (iu < m.size() && iu < u.size() && m[iu] == u[iu]) ++iu;
            // return prefix + "..."
            return m.substr(0, std::max(il, iu) ) + "*";
        }



        double confidence_interval(double confidence) const {
            return stdev() * sqrt(1.0 - confidence);
        }

        double confidence_interval_lower(double confidence) const {
            return mean() - confidence_interval(confidence);
        }

        double confidence_interval_upper(double confidence) const {
            return mean() + confidence_interval(confidence);
        }

        double confidence_interval_lower_stderr(double confidence) const {
            return stderror() * sqrt(1.0 - confidence);
        }

        double confidence_interval_upper_stderr(double confidence) const {
            return stderror() * sqrt(1.0 - confidence);
        }



        friend std::ostream & operator<< (std::ostream & out, const welford_variance & welford) {
            out << "(n=" << welford._nSamples <<
                   ", µ=" << (float) welford.mean() <<
                   ", σ=" << (float) welford.stdev() <<
                   ", SE=" << (float) welford.stderror() << ")";
            return out;
        }

    };
}

#endif //MERGESORTS_WELFORD_H
