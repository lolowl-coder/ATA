#include "strategy/MovingAverageStrategy.hpp"

StrategyDecision MovingAverageStrategy::evaluate(
    const StrategyContext& ctx,
    const IndicatorSet& indicators
) const
{
    if (!indicators.has(indicators::ema5)) {
        return {StrategyAction::Hold, 0.0};
    }

    const double ma = indicators.get(indicators::ema5);

    // TEMPORARY sizing rule (explicitly acknowledged)
    const double baseQty = 1.0;
	const double confidence = 1.0;
    const double qty = baseQty * confidence;

    if (ctx.closePrice > ma) {
        return {StrategyAction::Buy, confidence, qty};
    }

    if (ctx.closePrice < ma) {
        return {StrategyAction::Sell, confidence, qty};
    }

    return {StrategyAction::Hold, confidence, qty};
}
