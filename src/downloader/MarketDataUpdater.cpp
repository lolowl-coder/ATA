#include "downloader/MarketDataUpdater.hpp"

#include <iostream>
#include <stdexcept>

namespace {

    std::chrono::system_clock::time_point startOfUtcDay(
        const std::chrono::system_clock::time_point& tp
    )
    {
        using namespace std::chrono;
        const auto secs = duration_cast<seconds>(tp.time_since_epoch());
        const auto days = secs.count() / 86400;
        return system_clock::time_point{ seconds(days * 86400) };
    }

}

MarketDataUpdater::MarketDataUpdater(
    AlphaVantageDownloader& downloader,
    SQLiteStorage& storage,
    size_t dailyRequestLimit
)
    : mDownloader(downloader)
    , mStorage(storage)
    , mDailyRequestLimit(dailyRequestLimit)
{
}

bool MarketDataUpdater::needsUpdate(
    const std::string& symbol,
    Timeframe timeframe,
    const std::chrono::system_clock::time_point& nowUtc
) const
{
    const auto lastTs = mStorage.lastTimestamp(symbol, timeframe);
    if(!lastTs.has_value()) {
        return true;
    }

    const auto lastDay = startOfUtcDay(*lastTs);
    const auto today   = startOfUtcDay(nowUtc);

    return lastDay < today;
}

void MarketDataUpdater::updateDailyCompact(const std::string& symbol)
{
    const Timeframe tf = Timeframe::Daily;

    auto lastTs = storage.getLatestTimestamp(symbol, tf);

    auto json = downloader.download(symbol, /*compact=*/true);

    MarketSeries marketSeries = AlphaVantageParser::parse(json, symbol, tf, lastTs);

    for (const auto& bar : bars) {
        if (lastTs && bar.ts <= *lastTs)
            continue;

        storage.insertBar(symbol, tf, bar);
    }
}