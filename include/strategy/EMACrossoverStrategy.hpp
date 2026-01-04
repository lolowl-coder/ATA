#pragma once

#include "strategy/Strategy.hpp"

class EMACrossoverStrategy : public Strategy
{
public:
    EMACrossoverStrategy(
        size_t fastPeriod,
        size_t slowPeriod,
        size_t volatilityWindow,
        double minVolatility)
        :
        mFastEma(fastPeriod),
        mSlowEma(slowPeriod),
        mVolatilityWindow(volatilityWindow),
        mMinVolatility(minVolatility)
    {
    }

    std::vector<IndicatorKey> EMACrossoverStrategy::requiredIndicators() const override;

    virtual StrategyDecision evaluate(
        const StrategyContext& ctx,
        const IndicatorSet& indicators
    ) const override;

private:
    size_t mFastEma;
    size_t mSlowEma;
    size_t mVolatilityWindow;
    double mMinVolatility;
    RollingVolatility mVolatility;

    double mPrevFast = 0.0;
    double mPrevSlow = 0.0;
};
