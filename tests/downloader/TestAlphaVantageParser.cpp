#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "downloader/AlphaVantageParser.hpp"

#include <doctest/doctest.h>

#include <string>

TEST_CASE("Alpha Vantage daily parsing")
{
    const std::string json = R"json(
    {
        "Time Series (Daily)": {
            "2023-10-03": {
                "1. open": "170.00",
                "2. high": "172.00",
                "3. low": "168.50",
                "4. close": "171.20",
                "5. volume": "1234567"
            },
            "2023-10-02": {
                "1. open": "168.00",
                "2. high": "171.00",
                "3. low": "167.00",
                "4. close": "170.50",
                "5. volume": "1000000"
            }
        }
    }
    )json";

    const auto series = AlphaVantageParser::parse(
        json,
        "AAPL",
        Timeframe::Daily
    );

    CHECK(series.symbol == "AAPL");
    CHECK(series.bars.size() == 2);

    CHECK(series.bars[0].close == doctest::Approx(170.50));
    CHECK(series.bars[1].close == doctest::Approx(171.20));
}

TEST_CASE("Bars are sorted ascending by time")
{
    const std::string json = R"json(
    {
        "Time Series (Daily)": {
            "2023-10-02": {
                "1. open": "1",
                "2. high": "1",
                "3. low": "1",
                "4. close": "1",
                "5. volume": "1"
            },
            "2023-10-01": {
                "1. open": "2",
                "2. high": "2",
                "3. low": "2",
                "4. close": "2",
                "5. volume": "2"
            }
        }
    }
    )json";

    const auto series = AlphaVantageParser::parse(
        json,
        "TEST",
        Timeframe::Daily
    );

    CHECK(series.bars.front().close == 2.0);
    CHECK(series.bars.back().close == 1.0);
}
