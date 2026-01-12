#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "storage/SQLiteStorage.hpp"

#include <doctest/doctest.h>

TEST_CASE("SQLiteStorage store/load")
{
    SQLiteStorage storage(":memory:");

    MarketSeries s;
    s.symbol = "AAPL";
    s.timeframe = Timeframe::Daily;

    s.bars.push_back({
        TimePoint{ std::chrono::seconds(1000) },
        1,2,0.5,1.5,100
    });

    storage.store(s);

    const auto loaded = storage.load("AAPL", Timeframe::Daily);

    CHECK(loaded.bars.size() == 1);
    CHECK(loaded.bars[0].close == doctest::Approx(1.5));
}

TEST_CASE("SQLiteStorage resume timestamp")
{
    SQLiteStorage storage(":memory:");

    MarketSeries s;
    s.symbol = "AAPL";
    s.timeframe = Timeframe::Daily;

    s.bars.push_back({
        TimePoint{ std::chrono::seconds(100) },
        1,1,1,1,1
    });

    s.bars.push_back({
        TimePoint{ std::chrono::seconds(200) },
        2,2,2,2,2
    });

    storage.store(s);

    const auto ts = storage.lastTimestamp("AAPL", Timeframe::Daily);

    REQUIRE(ts.has_value());
    CHECK(ts->time_since_epoch() == std::chrono::seconds(200));
}
