#include <doctest/doctest.h>

#include "Strategy/MovingAverageCrossoverStrategy.hpp"
#include "Indicators/IndicatorSet.hpp"

TEST_CASE("MA crossover BUY only on crossing bar")
{
    MovingAverageCrossoverStrategy strategy(5, 10);
    StrategyContext ctx{};

    IndicatorSet ind;

    ind.set(IndicatorId::SMA, 5, 99.0);
    ind.set(IndicatorId::SMA, 10, 100.0);
    CHECK(strategy.evaluate(ctx, ind).action == StrategyAction::Hold);

    ind.set(IndicatorId::SMA, 5, 101.0);
    ind.set(IndicatorId::SMA, 10, 100.0);
    CHECK(strategy.evaluate(ctx, ind).action == StrategyAction::Buy);

    ind.set(IndicatorId::SMA, 5, 102.0);
    ind.set(IndicatorId::SMA, 10, 100.0);
    CHECK(strategy.evaluate(ctx, ind).action == StrategyAction::Hold);
}

TEST_CASE("MA crossover SELL only on crossing bar")
{
    MovingAverageCrossoverStrategy strategy(5, 10);
    StrategyContext ctx{};

    IndicatorSet ind;

    ind.set(IndicatorId::SMA, 5, 101.0);
    ind.set(IndicatorId::SMA, 10, 100.0);
    CHECK(strategy.evaluate(ctx, ind).action == StrategyAction::Hold);

    ind.set(IndicatorId::SMA, 5, 99.0);
    ind.set(IndicatorId::SMA, 10, 100.0);
    CHECK(strategy.evaluate(ctx, ind).action == StrategyAction::Sell);

    ind.set(IndicatorId::SMA, 5, 98.0);
    ind.set(IndicatorId::SMA, 10, 100.0);
    CHECK(strategy.evaluate(ctx, ind).action == StrategyAction::Hold);
}
