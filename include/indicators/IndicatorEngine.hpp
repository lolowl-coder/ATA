#pragma once

#include "core/MarketData.hpp"
#include "indicators/IndicatorSet.hpp"

class IndicatorEngine {
public:
    virtual ~IndicatorEngine() = default;

    virtual IndicatorSet compute(
        const MarketSeries& series,
        size_t index,
        const std::vector<IndicatorKey>& required
    ) const = 0;
};
