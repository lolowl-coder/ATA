#include "strategy/MovingAverageCrossoverStrategy.hpp"

MovingAverageCrossoverStrategy::MovingAverageCrossoverStrategy(
    int fastPeriod,
    int slowPeriod
)
    : mFastPeriod(fastPeriod)
    , mSlowPeriod(slowPeriod)
{
}

std::vector<IndicatorKey>
MovingAverageCrossoverStrategy::requiredIndicators() const
{
    return {
        { IndicatorId::SMA, mFastPeriod },
        { IndicatorId::SMA, mSlowPeriod }
    };
}

StrategyDecision
MovingAverageCrossoverStrategy::evaluate(
    const StrategyContext&,
    const IndicatorSet& indicators
) const
{
    StrategyDecision decision;

    if(!indicators.has(IndicatorId::SMA, mFastPeriod) ||
       !indicators.has(IndicatorId::SMA, mSlowPeriod))
    {
        return decision;
    }

    const double fast = indicators.get(IndicatorId::SMA, mFastPeriod);
    const double slow = indicators.get(IndicatorId::SMA, mSlowPeriod);

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

    if(crossUp)
    {
        decision.action = StrategyAction::Buy;
        decision.confidence = (fast - slow) / slow;
    }
    else if(crossDown)
    {
        decision.action = StrategyAction::Sell;
        decision.confidence = (slow - fast) / slow;
    }

    mPrevFast = fast;
    mPrevSlow = slow;

    return decision;
}
