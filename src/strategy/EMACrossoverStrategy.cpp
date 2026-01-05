#include "strategy/EMACrossoverStrategy.hpp"

std::vector<IndicatorKey> EMACrossoverStrategy::requiredIndicators() const
{
    return {
        { IndicatorId::EMA, mFastPeriod },
        { IndicatorId::EMA, mSlowPeriod },
        { IndicatorId::Volatility, mVolatilityPeriod }
    };
}

StrategyDecision EMACrossoverStrategy::evaluate(
    const StrategyContext& ctx,
    const IndicatorSet& indicators
) const
{
    StrategyDecision decision = {};

	const double fast =
		indicators.get({ IndicatorId::EMA, mFastPeriod });

	const double slow =
		indicators.get({ IndicatorId::EMA, mSlowPeriod });

	const double prevFast =
		indicators.get({ IndicatorId::EMA, mFastPeriod });

	const double prevSlow =
		indicators.get({ IndicatorId::EMA, mSlowPeriod });

    const double volatility =
        indicators.get({ IndicatorId::Volatility, mVolatilityPeriod });

    if (volatility < mMinVolatility) {
        decision.action = StrategyAction::Hold;
    }
    else {
        const bool crossedUp =
            prevFast <= prevSlow &&
            fast > slow;

        const bool crossedDown =
            prevFast >= prevSlow &&
            fast < slow;

        if (crossedUp && !ctx.hasPosition) {
            decision.action = StrategyAction::Buy;
        }
        else {
            if (crossedDown && ctx.hasPosition) {
                decision.action = StrategyAction::Sell;
            }
        }
    }

    return decision;
}
