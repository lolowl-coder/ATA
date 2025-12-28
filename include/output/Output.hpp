#pragma once

#include "core/Types.hpp"
#include "portfolio/Portfolio.hpp"
#include "strategy/StrategyContext.hpp"
#include "strategy/StrategyDecision.hpp"

#include <string>

struct OutputEventContext
{
    TimePoint ts;
    const StrategyContext strategyCtx;
    const StrategyDecision& decision;
    const Portfolio portfolio;
};

class Output {
public:
    virtual ~Output() = default;

    virtual void onBar(const OutputEventContext& ctx, const PriceSnapshot& priceSnapshot) {}
    virtual void onTrade(const OutputEventContext& ctx, const PriceSnapshot& priceSnapshot) {}
    virtual void onFinish(const Portfolio& portfolio, const PriceSnapshot& priceSnapshot) = 0;
};
