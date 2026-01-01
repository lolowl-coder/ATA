#include <doctest/doctest.h>

#include "Strategy/MovingAverageCrossoverStrategy.hpp"
#include "Indicators/IndicatorSet.hpp"

TEST_CASE("Position-aware BUY only when flat")
{
    MovingAverageCrossoverStrategy strategy(5, 10, 14);
    StrategyContext ctx{100.0, 0, false};

    IndicatorSet ind;

    ind.set(indicators::smaFast, 99.0);
    ind.set(indicators::smaSlow, 100.0);
    ind.set(indicators::volat14, 2.0);
    CHECK(strategy.evaluate(ctx, ind).action == StrategyAction::Hold);

    ind.set(indicators::smaFast, 101.0);
    ind.set(indicators::smaSlow, 100.0);
    const auto d = strategy.evaluate(ctx, ind);
    CHECK(d.action == StrategyAction::Buy);
    CHECK(d.confidence > 0.0);
}

TEST_CASE("No BUY if already in position")
{
    MovingAverageCrossoverStrategy strategy(5, 10, 14);
    IndicatorSet ind;

    ind.set(indicators::smaFast, 99.0);
    ind.set(indicators::smaSlow, 100.0);
    ind.set(indicators::volat14, 2.0);

    StrategyContext ctx{100.0, 0, true};
    strategy.evaluate(ctx, ind);

    ind.set(indicators::smaFast, 101.0);
    ind.set(indicators::smaSlow, 100.0);

    CHECK(strategy.evaluate(ctx, ind).action == StrategyAction::Hold);
}

TEST_CASE("Position-aware SELL only when holding")
{
    MovingAverageCrossoverStrategy strategy(5, 10, 14);
    IndicatorSet ind;

    ind.set(indicators::smaFast, 101.0);
    ind.set(indicators::smaSlow, 100.0);
    ind.set(indicators::volat14, 2.0);

    StrategyContext ctx{100.0, 0, true};
    strategy.evaluate(ctx, ind);

    ind.set(indicators::smaFast, 99.0);
    ind.set(indicators::smaSlow, 100.0);

    CHECK(strategy.evaluate(ctx, ind).action == StrategyAction::Sell);
}