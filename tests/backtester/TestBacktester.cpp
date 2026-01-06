#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "backtester/Backtester.hpp"
#include "backtester/FixedFractionalAllocator.hpp"
#include "core/MarketData.hpp"
#include "indicators/BasicIndicatorEngine.hpp"
#include "output/ConsoleOutput.hpp"
#include "portfolio/Portfolio.hpp"
#include "strategy/Strategy.hpp"

#include <doctest/doctest.h>

class DummyStrategy final : public Strategy {
public:
    explicit DummyStrategy(StrategyAction action, double confidence, double quantity)
        : action_(action)
        , confidence_(confidence)
        , quantity_(quantity)
    {}

    std::vector<IndicatorKey> requiredIndicators() const override {
        return { { IndicatorId::Volatility, 1, 0.0 } }; // no indicators needed
    }

    StrategyDecision evaluate(
        const StrategyContext&,
        const IndicatorSet&) const override {
        return StrategyDecision{ action_, confidence_, quantity_ };
    }

private:
    StrategyAction action_;
    double confidence_;
    double quantity_;
};

TEST_CASE("Portfolio basic trades") {
    Portfolio p(1000.0);
    p.trade("AAPL", 10, 10); // Buy 10 shares at $10
    CHECK(p.cash() == 900.0);
    CHECK(p.getPosition("AAPL").quantity == 10);
}

TEST_CASE("Backtester applies DummyStrategy decisions") {
    Portfolio portfolio(100.0);
    BasicIndicatorEngine engine;
    ConsoleOutput output;
    Backtester bt(portfolio, engine, output);
    const std::string symbol = "TEST";
    MarketSeries series = {
        .symbol = symbol,
        .bars = {
            { std::chrono::system_clock::now(), 10, 10, 10, 10, 0 },
            { std::chrono::system_clock::now(), 10, 10, 10, 10, 0 }
        }
    };
    MarketData marketData = { series };

    DummyStrategy alwaysBuy(
        StrategyAction::Buy, 1.0, 1.0);

	FixedFractionalAllocator capitalAllocator(0.2, 4); // 20% risk fraction, max 4 positions

    bt.run(symbol, marketData, alwaysBuy, capitalAllocator, 0, series.bars.size() - 1);

    // According to current allocation config
    CHECK(portfolio.getPosition(symbol).quantity == 0.975);
    CHECK(portfolio.cash() == doctest::Approx(90.25));
    CHECK(portfolio.tradeCount() == 2);
}
