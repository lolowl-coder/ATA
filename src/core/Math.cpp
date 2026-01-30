#include "core/Math.hpp"

#include <cmath>

bool isZero(double x, double tol)
{
    return std::abs(x) < tol;
}

double percentile(
    const std::vector<double>& values,
    const size_t window,
    const double p)
{
    if(values.empty())
    {
        return 0.0;
        //throw std::invalid_argument("percentile: empty input");
    }

    if(p < 0.0 || p > 1.0)
    {
        return 0.0;
        //throw std::invalid_argument("percentile: p must be in [0, 1]");
    }

    if(values.size() < window)
    {
        return 0.0;
	}

    std::vector<double> sorted(values.end() - window, values.end());
    std::sort(sorted.begin(), sorted.end());

    const size_t n = sorted.size() - 1;

    // Nearest-rank method
    size_t index = std::clamp<size_t>(static_cast<size_t>(std::ceil(p * n)), 0, n);

    return sorted[index];
}