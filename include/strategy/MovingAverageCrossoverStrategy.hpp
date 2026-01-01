#pragma once

#include "strategy/Strategy.hpp"
#include "indicators/IndicatorSet.hpp"

class MovingAverageCrossoverStrategy final : public Strategy
{
public:
    MovingAverageCrossoverStrategy(int fastPeriod, int slowPeriod);

    std::vector<IndicatorKey> requiredIndicators() const override;

    StrategyDecision evaluate(
        const StrategyContext& ctx,
        const IndicatorSet& indicators
    ) const override;

private:
    int mFastPeriod;
    int mSlowPeriod;

    mutable bool mHasPrev = false;
    mutable double mPrevFast = 0.0;
    mutable double mPrevSlow = 0.0;
};
