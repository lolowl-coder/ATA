#pragma once

#include "indicators/IndicatorEngine.hpp"
#include "indicators/Indicators.hpp"
#include "indicators/IndicatorSet.hpp"

#include <vector>

class BasicIndicatorEngine : public IndicatorEngine {
public:
    virtual IndicatorSet compute(
        const MarketSeries& series,
        size_t index,
        const std::vector<IndicatorKey>& required
    ) override;

    virtual void resetVolatilityHistory() override { mVolatilityHistory.clear(); }
    virtual void resetATRHistory() override { mATRHistory.clear(); }

private:
    std::vector<double> mVolatilityHistory;
    std::vector<double> mATRHistory;
};
