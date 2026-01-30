#pragma once

#include "strategy/Strategy.hpp"
#include "indicators/IndicatorSet.hpp"

class CrossoverStrategy final : public Strategy
{
public:
    CrossoverStrategy(
        IndicatorKey fast,
        IndicatorKey slow,
        IndicatorKey volatFast,
        IndicatorKey volatSlow,
        IndicatorKey perc,
        IndicatorKey atr,
        IndicatorKey atrPercentile);

    std::vector<IndicatorKey> requiredIndicators() const override;

    StrategyDecision evaluate(
        const StrategyContext& ctx,
        const IndicatorSet& indicators
    ) const override;

private:
    IndicatorKey mFast;
    IndicatorKey mSlow;
    IndicatorKey mVolatFast;
    IndicatorKey mVolatSlow;
    IndicatorKey mPercentile;
    IndicatorKey mAtr;
    IndicatorKey mAtrPercentile;
    const double mMinAtr = 0.002;
    const double mMinVolat = 0.5;
};
