#pragma once

#include "core/MarketData.hpp"

#include <vector>
#include <cstddef>

namespace Indicators
{
	double sma(const std::vector<double>& values, size_t window);

	double ema(const std::vector<double>& values, size_t window);

	double rsi(const std::vector<double>& values, size_t window);

	double volatility(const std::vector<double>& values, size_t window);

	double volatilityPercentile(const std::vector<double>& values, size_t window, const double P);

	double ATR(const std::vector<Bar>& bars, const size_t window);

	double ATRPercentile(const std::vector<double>& atrHistory, const size_t window, const double P);
}
