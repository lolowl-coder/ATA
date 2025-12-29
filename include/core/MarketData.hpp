#pragma once

#include "Enums.hpp"
#include "core/Types.hpp"

#include <string>
#include <vector>

struct Bar {
    TimePoint ts;
    double open = 0.0;
    double high = 0.0;
    double low = 0.0;
    double close = 0.0;
    double volume = 0.0;
};

using Bars = std::vector<Bar>;

struct MarketSeries {
    std::string symbol;
    Timeframe timeframe = Timeframe::Daily;
    std::vector<Bar> bars;
};

using MarketData = std::vector<MarketSeries>;