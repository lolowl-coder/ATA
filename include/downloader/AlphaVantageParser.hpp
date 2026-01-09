#pragma once

#include <string>
#include <chrono>
#include <vector>
#include <stdexcept>

#include "core/MarketData.hpp"

class AlphaVantageParser {
public:
    static MarketSeries parse(
        const std::string& jsonText,
        const std::string& symbol,
        Timeframe timeframe
    );
};
