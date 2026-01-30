#include "core/Math.hpp"
#include "indicators/IndicatorSet.hpp"
#include "strategy/CrossoverStrategy.hpp"

#include <iostream>

CrossoverStrategy::CrossoverStrategy(
    IndicatorKey fast,
    IndicatorKey slow,
    IndicatorKey volat14,
    IndicatorKey volat60,
	IndicatorKey percentile,
	IndicatorKey atr,
	IndicatorKey atrPercentile
)
    : mFast(fast)
    , mSlow(slow)
    , mVolatFast(volat14)
    , mVolatSlow(volat60)
    , mPercentile(percentile)
    , mAtr(atr)
    , mAtrPercentile(atrPercentile)
{
}

std::vector<IndicatorKey>
CrossoverStrategy::requiredIndicators() const
{
    return { mFast, mSlow, mVolatFast, mVolatSlow, mAtr, mAtrPercentile };
}

StrategyDecision
CrossoverStrategy::evaluate(
    const StrategyContext& ctx,
    const IndicatorSet& indicators
) const
{
    const double fast = indicators.get(mFast);
    const double slow = indicators.get(mSlow);
    const double volatFast = indicators.get(mVolatFast);
    const double volatSlow = indicators.get(mVolatSlow);
    const double percentile = indicators.get(mPercentile);
    const double atr = indicators.get(mAtr);
    const double atrPercentile = indicators.get(mPercentile);
    const double normalizedAtr = atr / ctx.closePrice;
	const double minNormAtr = 0.018;

    StrategyDecision decision = {};
    
    //if(volatFast / volatSlow >= 1.0)
    //if(volatFast > percentile)
    {
        if (
            fast > slow
            && !ctx.hasPosition
            && atr >= atrPercentile
            && volatFast > mMinVolat
            //&& ctx.closePrice > volatSlow
			//&& volatFast / volatSlow >= 1.0
            //&& normalizedAtr >= minNormAtr
            ) {
            decision.action = StrategyAction::Buy;
            decision.confidence = std::abs(fast - slow) / atr;
        }

        if (fast < slow && ctx.hasPosition) {
            decision.action = StrategyAction::Sell;
            decision.confidence = std::abs(slow - fast) / atr;
        }
    }

    /*std::cout
        << " vol " << vol << " minVol " << minVolatility
		<< " close " << ctx.closePrice
        << " fast " << fast
        << " slow " << slow
        << " vol " << vol
        << " decision " << strategyActionToString(decision.action)
		<< std::endl;*/

    return decision;
}
