#pragma once

#include <vector>
#include <cstddef>

namespace Indicators
{

	double sma(const std::vector<double>& values, size_t window);

	double rsi(const std::vector<double>& values, size_t window);

	double volatility(const std::vector<double>& values, size_t window);

}
