#pragma once

#include "strategy/Strategy.hpp"

class MovingAverageStrategy : public Strategy {
public:
    virtual std::vector<IndicatorKey> requiredIndicators() const
    {
        return { { IndicatorId::SMA, 5 } };
    }

    virtual StrategyDecision evaluate(
        const StrategyContext& ctx,
        const IndicatorSet& indicators
    ) const override;
};
