#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "core/MarketData.hpp"
#include "indicators/Indicators.hpp"
#include "indicators/BasicIndicatorEngine.hpp"

#include <doctest/doctest.h>

TEST_CASE("SMA full window") {
    std::vector<double> v{1, 2, 3, 4, 5};
    CHECK(Indicators::sma(v, 5) == doctest::Approx(3.0));
}

TEST_CASE("SMA sliding window") {
    std::vector<double> v{1, 2, 3, 4, 5};
    CHECK(Indicators::sma(v, 3) == doctest::Approx(4.0));
}

TEST_CASE("Volatility zero") {
    std::vector<double> v{5, 5, 5, 5};
    CHECK(Indicators::volatility(v, 4) == doctest::Approx(0.0));
}

TEST_CASE("SMA invalid window") {
    std::vector<double> v{1, 2, 3};
    CHECK_THROWS_AS(Indicators::sma(v, 5), std::invalid_argument);
}

TEST_CASE("RSI rising prices") {
    std::vector<double> v{1, 2, 3, 4, 5, 6};
    double r = Indicators::rsi(v, 5);
    CHECK(r > 70.0);
}

TEST_CASE("RSI falling prices") {
    std::vector<double> v{6, 5, 4, 3, 2, 1};
    double r = Indicators::rsi(v, 5);
    CHECK(r < 30.0);
}

MarketSeries makeLinearSeries(size_t count)
{
    MarketSeries series;
    for (size_t i = 1; i <= count; ++i) {
        series.bars.push_back(
            {
                .open  = double(i),
                .high  = double(i),
                .low   = double(i),
                .close = double(i)
            }
        );
    }
    return series;
}

TEST_CASE("BasicIndicatorEngine computes MA at given index")
{
	MarketSeries series = makeLinearSeries(10);

    // Last bar
    const size_t index = series.bars.size() - 1;

    BasicIndicatorEngine engine;
    IndicatorSet result = engine.compute(series, index, { indicators::smaFast });

    REQUIRE(result.has({ indicators::smaFast }));

    // Expected MA of [6,7,8,9,10]
    const double expected_ma =
        (6.0 + 7.0 + 8.0 + 9.0 + 10.0) / 5.0;

    CHECK(result.get(indicators::smaFast) == doctest::Approx(expected_ma));
}

TEST_CASE("BasicIndicatorEngine skips MA if index too small")
{
    MarketSeries series = makeLinearSeries(4);

    // Last bar
    const size_t index = series.bars.size() - 1;

    BasicIndicatorEngine engine;
    IndicatorSet result;
    CHECK_THROWS_AS(
        result = engine.compute(series, index, { indicators::smaFast }),
        std::invalid_argument);

    CHECK_FALSE(result.has(indicators::smaFast));
}

TEST_CASE("BasicIndicatorEngine computes multiple requested indicators")
{
    MarketSeries series = makeLinearSeries(30);

    BasicIndicatorEngine engine;
    const size_t index = series.bars.size() - 1;
    auto required = { indicators::smaSlow, indicators::rsi14 };
    IndicatorSet result = engine.compute(series, index, required);

    CHECK(result.has(indicators::smaSlow));
    CHECK(result.has(indicators::rsi14));
}

TEST_CASE("BasicIndicatorEngine does not compute unrequested indicators")
{
    MarketSeries series =
    {
        .bars = Bars(50, {.close = 100.0})
    };

    const size_t index = series.bars.size() - 1;

    BasicIndicatorEngine engine;
    IndicatorSet result = engine.compute(series, index, { indicators::smaFast });

    CHECK(result.has(indicators::smaFast));
    CHECK_FALSE(result.has(indicators::rsi14));
    CHECK_FALSE(result.has(indicators::volat20));
}
