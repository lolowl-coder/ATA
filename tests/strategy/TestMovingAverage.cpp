#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "strategy/MovingAverageStrategy.hpp"
#include "indicators/IndicatorSet.hpp"

#include <doctest/doctest.h>

TEST_CASE("MA strategy emits Buy when price above MA")
{
    IndicatorSet indicators;
    indicators.set(indicators::smaFast5, 100.0);

    StrategyContext ctx;
    ctx.closePrice = 105.0;
    ctx.index = 10;

    MovingAverageStrategy strategy;
    StrategyDecision decision = strategy.evaluate(ctx, indicators);

    CHECK(decision.action == StrategyAction::Buy);
}

TEST_CASE("MA strategy emits Sell when price below MA")
{
    IndicatorSet indicators;
    indicators.set(indicators::smaFast5, 100.0);

    StrategyContext ctx;
    ctx.closePrice = 95.0;
    ctx.index = 10;

    MovingAverageStrategy strategy;
    StrategyDecision decision = strategy.evaluate(ctx, indicators);

    CHECK(decision.action == StrategyAction::Sell);
}

TEST_CASE("MA strategy holds when MA missing")
{
    IndicatorSet indicators; // empty

    StrategyContext ctx;
    ctx.closePrice = 100.0;
    ctx.index = 10;

    MovingAverageStrategy strategy;
    StrategyDecision decision = strategy.evaluate(ctx, indicators);

    CHECK(decision.action == StrategyAction::Hold);
}
