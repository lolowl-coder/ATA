#pragma once

#include "core/MarketData.hpp"
#include "indicators/IndicatorSet.hpp"
#include "strategy/StrategyContext.hpp"
#include "strategy/StrategyDecision.hpp"
#include "Enums.hpp"

#include <vector>

class Strategy {
public:
    virtual ~Strategy() = default;

    virtual std::vector<IndicatorKey> requiredIndicators() const = 0;

    virtual StrategyDecision evaluate(
        const StrategyContext& ctx,
        const IndicatorSet& indicators
    ) const = 0;
};
