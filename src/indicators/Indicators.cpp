#include "indicators/Indicators.hpp"

#include <cmath>
#include <numeric>
#include <stdexcept>

namespace Indicators {

double sma(const std::vector<double>& values, size_t window)
{
    if (window == 0 || values.size() < window) {
        throw std::invalid_argument("SMA: invalid window size");
    }

    double sum = 0.0;
    for (size_t i = values.size() - window; i < values.size(); ++i) {
        sum += values[i];
    }

    return sum / static_cast<double>(window);
}

double volatility(const std::vector<double>& values, size_t window)
{
    if (window == 0 || values.size() < window) {
        throw std::invalid_argument("Volatility: invalid window size");
    }

    const double mean = sma(values, window);

    double variance = 0.0;
    for (size_t i = values.size() - window; i < values.size(); ++i) {
        const double diff = values[i] - mean;
        variance += diff * diff;
    }

    variance /= static_cast<double>(window);

    return std::sqrt(variance);
}

double rsi(const std::vector<double>& values, size_t window)
{
    if (window == 0 || values.size() < window + 1) {
        throw std::invalid_argument("RSI: invalid window size");
    }

    double gain = 0.0;
    double loss = 0.0;

    // Use last `window` deltas
    for (size_t i = values.size() - window; i < values.size(); ++i) {
        const double delta = values[i] - values[i - 1];
        if (delta > 0.0) {
            gain += delta;
        } else {
            loss -= delta; // delta is negative
        }
    }

    gain /= static_cast<double>(window);
    loss /= static_cast<double>(window);

    if (loss == 0.0) {
        return 100.0;
    }

    const double rs = gain / loss;
    return 100.0 - (100.0 / (1.0 + rs));
}

} // namespace Indicators
