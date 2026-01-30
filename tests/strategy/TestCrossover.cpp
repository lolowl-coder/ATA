#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "strategy/CrossoverStrategy.hpp"
#include "indicators/IndicatorSet.hpp"

#include <doctest/doctest.h>

TEST_CASE("Position-aware BUY only when flat")
{
    CrossoverStrategy strategy(
        indicators::sma5, indicators::sma10,
        indicators::volat14, indicators::volat60, indicators::volatPercentile50_50,
        indicators::atr14, indicators::atrPercentile14_40);
    StrategyContext ctx{100.0, 0, 0, false};

    IndicatorSet ind;

    ind.set(indicators::sma5, 99.0);
    ind.set(indicators::sma10, 100.0);
    ind.set(indicators::volat14, 2.0);
    CHECK(strategy.evaluate(ctx, ind).action == StrategyAction::Hold);

    ind.set(indicators::sma5, 101.0);
    ind.set(indicators::sma10, 100.0);
    const auto d = strategy.evaluate(ctx, ind);
    CHECK(d.action == StrategyAction::Buy);
    CHECK(d.confidence > 0.0);
}

TEST_CASE("No BUY if already in position")
{
    CrossoverStrategy strategy(indicators::sma5, indicators::sma10, indicators::volat14,
        indicators::volat60, indicators::volatPercentile50_50,
        indicators::atr14, indicators::atrPercentile14_40);
    IndicatorSet ind;

    ind.set(indicators::sma5, 99.0);
    ind.set(indicators::sma10, 100.0);
    ind.set(indicators::volat14, 2.0);

    StrategyContext ctx{100.0, 0, 0, true};
    strategy.evaluate(ctx, ind);

    ind.set(indicators::sma5, 101.0);
    ind.set(indicators::sma10, 100.0);

    CHECK(strategy.evaluate(ctx, ind).action == StrategyAction::Hold);
}

TEST_CASE("Position-aware SELL only when holding")
{
    CrossoverStrategy strategy(indicators::sma5, indicators::sma10, indicators::volat14, indicators::volat14,
        indicators::volatPercentile50_50,
        indicators::atr14, indicators::atrPercentile14_40);
    IndicatorSet ind;

    ind.set(indicators::sma5, 101.0);
    ind.set(indicators::sma10, 100.0);
    ind.set(indicators::volat14, 2.0);

    StrategyContext ctx{100.0, 0, 0, true};
    strategy.evaluate(ctx, ind);

    ind.set(indicators::sma5, 99.0);
    ind.set(indicators::sma10, 100.0);

    CHECK(strategy.evaluate(ctx, ind).action == StrategyAction::Sell);
}