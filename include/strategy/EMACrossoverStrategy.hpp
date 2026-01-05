#pragma once

#include "indicators/IndicatorSet.hpp"
#include "strategy/Strategy.hpp"
#include "Enums.hpp"

#include <vector>

class EMACrossoverStrategy : public Strategy
{
public:
    EMACrossoverStrategy(
        int fastPeriod,
        int slowPeriod,
        int volatilityWindow,
        double minVolatility)
        :
        mFastPeriod(fastPeriod),
        mSlowPeriod(slowPeriod),
        mVolatilityPeriod(volatilityWindow),
        mMinVolatility(minVolatility)
    {
    }

    std::vector<IndicatorKey> requiredIndicators() const override;

    virtual StrategyDecision evaluate(
        const StrategyContext& ctx,
        const IndicatorSet& indicators
    ) const override;

private:
    int mFastPeriod;
    int mSlowPeriod;
    int mVolatilityPeriod;
    double mMinVolatility;

    double mPrevFast = 0.0;
    double mPrevSlow = 0.0;
};
