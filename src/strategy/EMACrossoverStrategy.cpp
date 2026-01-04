#include "strategy/EMACrossoverStrategy.hpp"

std::vector<IndicatorKey> EMACrossoverStrategy::requiredIndicators() const
{
    return {
        IndicatorKey::ema({ IndicatorId::SMA, mFastPeriod } ),
        IndicatorKey::ema({ IndicatorId::SMA, mSlowPeriod } ),
        IndicatorKey::volatility(mVolatilityWindow)
    };
}

StrategyDecision MaCrossoverStrategy::evaluate(
    const StrategyContext& ctx,
    const IndicatorSet& indicators
) const
{
    const double fast =
        indicators.get( { IndicatorId::SMA, mFastPeriod } );

    const double slow =
        indicators.get( { IndicatorId::SMA, mSlowPeriod } );

    const double prevFast =
        indicators.getPrev("ema_fast");

    const double prevSlow =
        indicators.getPrev("ema_slow");

    const double volatility =
        indicators.get("volatility");

    if(volatility < mMinVolatility)
        return StrategyDecision::none();

    const bool crossedUp =
        prevFast <= prevSlow &&
        fast > slow;

    const bool crossedDown =
        prevFast >= prevSlow &&
        fast < slow;

    if(crossedUp && !ctx.hasPosition)
        return StrategyDecision::buy();

    if(crossedDown && ctx.hasPosition)
        return StrategyDecision::sell();

    return StrategyDecision::none();
}
