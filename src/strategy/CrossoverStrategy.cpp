#include "indicators/IndicatorSet.hpp"
#include "strategy/CrossoverStrategy.hpp"

CrossoverStrategy::CrossoverStrategy(
    IndicatorKey fast,
    IndicatorKey slow,
    IndicatorKey volatility,
    double minVolatility
)
    : mFast(fast)
    , mSlow(slow)
    , mVolatility(volatility)
    , mMinVolatility(minVolatility)
{
}

std::vector<IndicatorKey>
CrossoverStrategy::requiredIndicators() const
{
    return { mFast, mSlow, mVolatility };
}

StrategyDecision
CrossoverStrategy::evaluate(
    const StrategyContext& ctx,
    const IndicatorSet& indicators
) const
{

    const double fast = indicators.get(mFast);
    const double slow = indicators.get(mSlow);
    const double vol = indicators.get(mVolatility);

    StrategyDecision decision = {};
    
    if(vol >= mMinVolatility)
    {
        if (fast > slow && !ctx.hasPosition) {
            decision.action = StrategyAction::Buy;
            decision.confidence = std::abs(fast - slow) / vol;
        }

        if (fast < slow && ctx.hasPosition) {
            decision.action = StrategyAction::Sell;
            decision.confidence = std::abs(slow - fast) / vol;
        }
    }

    return decision;
}
