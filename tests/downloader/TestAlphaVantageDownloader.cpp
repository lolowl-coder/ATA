#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "downloader/AlphaVantageDownloader.hpp"

#include <doctest/doctest.h>

class MockHttpClient final : public IHttpClient {
public:
    explicit MockHttpClient(std::string response)
        : mResponse(std::move(response))
    {
    }

    std::string get(const std::string&) const override {
        return mResponse;
    }

private:
    std::string mResponse;
};

TEST_CASE("AlphaVantageDownloader returns MarketSeries")
{
    const std::string json = R"json(
    {
        "Time Series (Daily)": {
            "2023-10-01": {
                "1. open": "10",
                "2. high": "11",
                "3. low": "9",
                "4. close": "10.5",
                "5. volume": "100"
            }
        }
    }
    )json";

    auto http = std::make_unique<MockHttpClient>(json);

    AlphaVantageDownloader dl(
        "dummyKey",
        std::move(http)
    );

    const auto series = dl.download(
        "TEST",
        Timeframe::Daily
    );

    CHECK(series.symbol == "TEST");
    CHECK(series.bars.size() == 1);
    CHECK(series.bars[0].close == doctest::Approx(10.5));
}