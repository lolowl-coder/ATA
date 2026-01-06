#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "strategy/CrossoverStrategy.hpp"
#include "indicators/IndicatorSet.hpp"

#include <doctest/doctest.h>

TEST_CASE("Position-aware BUY only when flat")
{
    CrossoverStrategy strategy(indicators::smaFast5, indicators::smaSlow10, indicators::volat14, 0.5);
    StrategyContext ctx{100.0, 0, 0, false};

    IndicatorSet ind;

    ind.set(indicators::smaFast5, 99.0);
    ind.set(indicators::smaSlow10, 100.0);
    ind.set(indicators::volat14, 2.0);
    CHECK(strategy.evaluate(ctx, ind).action == StrategyAction::Hold);

    ind.set(indicators::smaFast5, 101.0);
    ind.set(indicators::smaSlow10, 100.0);
    const auto d = strategy.evaluate(ctx, ind);
    CHECK(d.action == StrategyAction::Buy);
    CHECK(d.confidence > 0.0);
}

TEST_CASE("No BUY if already in position")
{
    CrossoverStrategy strategy(indicators::smaFast5, indicators::smaSlow10, indicators::volat14, 0.5);
    IndicatorSet ind;

    ind.set(indicators::smaFast5, 99.0);
    ind.set(indicators::smaSlow10, 100.0);
    ind.set(indicators::volat14, 2.0);

    StrategyContext ctx{100.0, 0, 0, true};
    strategy.evaluate(ctx, ind);

    ind.set(indicators::smaFast5, 101.0);
    ind.set(indicators::smaSlow10, 100.0);

    CHECK(strategy.evaluate(ctx, ind).action == StrategyAction::Hold);
}

TEST_CASE("Position-aware SELL only when holding")
{
    CrossoverStrategy strategy(indicators::smaFast5, indicators::smaSlow10, indicators::volat14, 0.5);
    IndicatorSet ind;

    ind.set(indicators::smaFast5, 101.0);
    ind.set(indicators::smaSlow10, 100.0);
    ind.set(indicators::volat14, 2.0);

    StrategyContext ctx{100.0, 0, 0, true};
    strategy.evaluate(ctx, ind);

    ind.set(indicators::smaFast5, 99.0);
    ind.set(indicators::smaSlow10, 100.0);

    CHECK(strategy.evaluate(ctx, ind).action == StrategyAction::Sell);
}