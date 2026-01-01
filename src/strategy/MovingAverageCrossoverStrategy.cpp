#include "strategy/MovingAverageCrossoverStrategy.hpp"

MovingAverageCrossoverStrategy::MovingAverageCrossoverStrategy(
    int fastPeriod,
    int slowPeriod,
    int volatilityPeriod
)
    : mFastPeriod(fastPeriod)
    , mSlowPeriod(slowPeriod)
    , mVolatilityPeriod(volatilityPeriod)
{
}

std::vector<IndicatorKey>
MovingAverageCrossoverStrategy::requiredIndicators() const
{
    return {
        { IndicatorId::SMA, mFastPeriod },
        { IndicatorId::SMA, mSlowPeriod },
        { IndicatorId::Volatility, mVolatilityPeriod }
    };
}

StrategyDecision
MovingAverageCrossoverStrategy::evaluate(
    const StrategyContext& ctx,
    const IndicatorSet& indicators
) const
{
    StrategyDecision decision = {};

    if(!indicators.has({ IndicatorId::SMA, mFastPeriod }) ||
        !indicators.has({ IndicatorId::SMA, mSlowPeriod }) ||
        !indicators.has({ IndicatorId::Volatility, mVolatilityPeriod }))
    {
        return decision;
    }

    const double fast = indicators.get({ IndicatorId::SMA, mFastPeriod });
    const double slow = indicators.get({ IndicatorId::SMA, mSlowPeriod });
    const double vol = indicators.get({ IndicatorId::Volatility, mVolatilityPeriod });

    if(vol <= 0.0)
    {
        return decision;
    }

    if(!mHasPrev)
    {
        mPrevFast = fast;
        mPrevSlow = slow;
        mHasPrev = true;
        return decision;
    }

    const bool crossUp =
        (mPrevFast <= mPrevSlow) && (fast > slow);

    const bool crossDown =
        (mPrevFast >= mPrevSlow) && (fast < slow);

    if(crossUp && !ctx.hasPosition)
    {
        decision.action = StrategyAction::Buy;
        decision.confidence = std::abs(fast - slow) / vol;
    }
    else if(crossDown && ctx.hasPosition)
    {
        decision.action = StrategyAction::Sell;
        decision.confidence = std::abs(slow - fast) / vol;
    }

    mPrevFast = fast;
    mPrevSlow = slow;

    return decision;
}
