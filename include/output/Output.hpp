#pragma once

#include "core/Types.hpp"
#include "portfolio/Portfolio.hpp"
#include "strategy/StrategyContext.hpp"
#include "strategy/StrategyDecision.hpp"

#include <string>

struct OutputEventContext
{
    const std::string symbol;
    const TimePoint ts;
    const StrategyContext strategyCtx;
    const StrategyDecision& decision;
    const Portfolio portfolio;
};

class Output {
public:
    virtual ~Output() = default;

    virtual void onBar(const OutputEventContext&, const PriceSnapshot&) {}
    virtual void onTrade(const OutputEventContext&, const PriceSnapshot&) {}
    virtual void onFinish(const Portfolio&, const PriceSnapshot&) = 0;
};
