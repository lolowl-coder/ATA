#pragma once

#include "strategy/Strategy.hpp"
#include "indicators/IndicatorSet.hpp"

class CrossoverStrategy final : public Strategy
{
public:
    CrossoverStrategy(
        IndicatorKey fast,
        IndicatorKey slow,
        IndicatorKey volatility,
        double minVolatility);

    std::vector<IndicatorKey> requiredIndicators() const override;

    StrategyDecision evaluate(
        const StrategyContext& ctx,
        const IndicatorSet& indicators
    ) const override;

private:
    IndicatorKey mFast;
    IndicatorKey mSlow;
    IndicatorKey mVolatility;
    double mMinVolatility;
};
