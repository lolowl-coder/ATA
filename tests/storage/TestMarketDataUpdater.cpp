#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "downloader/AlphaVantageParser.hpp"
#include "storage/MarketDataUpdater.hpp"

#include <doctest/doctest.h>

class FakeDownloader : public AlphaVantageDownloader {
public:
    FakeDownloader(std::string apiKey, std::unique_ptr<IHttpClient> http)
		: AlphaVantageDownloader(std::move(apiKey), std::move(http))
    {};

    MarketSeries download(
        const std::string& symbol,
        Timeframe timeFrame) const override {
        return AlphaVantageParser::parse(response, symbol, timeFrame);
    }

    std::string response;
};

TEST_CASE("MarketDataUpdater inserts all bars on empty DB")
{
    FakeDownloader dl("", nullptr);
    dl.response = R"json(
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
            },
            "2023-10-01": {
                "1. open": "165.00",
                "2. high": "169.00",
                "3. low": "163.00",
                "4. close": "164",
                "5. volume": "1001000"
            }
        }
    }
    )json";

    SQLiteStorage storage(":memory:");
    MarketDataUpdater updater(dl, storage);

    updater.updateDailyCompact("AAPL");

    CHECK(storage.countBars("AAPL", Timeframe::Daily) == 3);
}

TEST_CASE("MarketDataUpdater resumes from last timestamp")
{
    SQLiteStorage storage(":memory:");
    {
        FakeDownloader dl("", nullptr);
        dl.response = R"json(
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
                },
                "2023-10-01": {
                    "1. open": "165.00",
                    "2. high": "169.00",
                    "3. low": "163.00",
                    "4. close": "164",
                    "5. volume": "1001000"
                }
            }
        }
        )json";
        MarketDataUpdater updater(dl, storage);
        updater.updateDailyCompact("AAPL");
    }

    CHECK(storage.countBars("AAPL", Timeframe::Daily) == 3);

    {
        FakeDownloader dl("", nullptr);
        dl.response = R"json(
        {
            "Time Series (Daily)": {
                "2023-10-05": {
                    "1. open": "170.00",
                    "2. high": "172.00",
                    "3. low": "168.50",
                    "4. close": "171.20",
                    "5. volume": "1234567"
                },
                "2023-10-04": {
                    "1. open": "168.00",
                    "2. high": "171.00",
                    "3. low": "167.00",
                    "4. close": "170.50",
                    "5. volume": "1000000"
                },
                "2023-10-03": {
                    "1. open": "165.00",
                    "2. high": "169.00",
                    "3. low": "163.00",
                    "4. close": "164",
                    "5. volume": "1001000"
                }
            }
        }
        )json";
        MarketDataUpdater updater(dl, storage);
        updater.updateDailyCompact("AAPL");
    }

    CHECK(storage.countBars("AAPL", Timeframe::Daily) == 5);
}

