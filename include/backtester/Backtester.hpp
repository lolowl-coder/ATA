#pragma once

#include "core/MarketData.hpp"
#include "indicators/IndicatorEngine.hpp"
#include "output/Output.hpp"
#include "strategy/Strategy.hpp"
#include "portfolio/Portfolio.hpp"

#include <string>
#include <vector>

class Backtester {
public:
    Backtester(Portfolio& portfolio, IndicatorEngine& indicatorEngine, Output& output)
        : portfolio_(portfolio)
        , indicatorEngine_(indicatorEngine)
		, output_(output)
    {
    }

    // Run simulation on symbol data
    void run(const std::string& symbol, const MarketSeries& series, Strategy& strategy);

private:
    Portfolio& portfolio_;
    IndicatorEngine& indicatorEngine_;
	Output& output_;
};
