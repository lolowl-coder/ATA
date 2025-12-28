#pragma once

#include "indicators/IndicatorEngine.hpp"
#include "indicators/Indicators.hpp"
#include "indicators/IndicatorSet.hpp"

class BasicIndicatorEngine : public IndicatorEngine {
public:
    IndicatorSet compute(
        const MarketSeries& series,
        size_t index,
        const std::vector<IndicatorKey>& required
    ) const;
};
