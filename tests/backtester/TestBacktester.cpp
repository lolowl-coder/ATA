#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "backtester/Backtester.hpp"
#include "core/MarketData.hpp"
#include "indicators/BasicIndicatorEngine.hpp"
#include "output/ConsoleOutput.hpp"
#include "portfolio/Portfolio.hpp"
#include "strategy/Strategy.hpp"

#include <doctest/doctest.h>

class DummyStrategy final : public Strategy {
public:
    explicit DummyStrategy(StrategyAction action, double confidence)
        : action_(action)
        , confidence_(confidence)
    {}

    std::vector<IndicatorKey> requiredIndicators() const override {
        return {}; // no indicators needed
    }

    StrategyDecision evaluate(
        const StrategyContext&,
        const IndicatorSet&) const override {
        return StrategyDecision{ action_, confidence_ };
    }

private:
    StrategyAction action_;
    double confidence_;
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
    MarketSeries series = {
        .bars = {
            { std::chrono::system_clock::now(), 10, 10, 10, 10, 0 },
            { std::chrono::system_clock::now(), 10, 10, 10, 10, 0 }
        }
    };

    DummyStrategy alwaysBuy(StrategyAction::Buy, 1.0);

    bt.run("TEST", series, alwaysBuy);

    CHECK(portfolio.getPosition("TEST").quantity == 2.0);
    CHECK(portfolio.cash() == doctest::Approx(80.0));
}

