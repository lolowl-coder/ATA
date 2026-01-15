#include "downloader/AlphaVantageParser.hpp"
#include "storage/MarketDataUpdater.hpp"

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
    SQLiteStorage& storage
)
    : mDownloader(downloader)
    , mStorage(storage)
{
}

void MarketDataUpdater::updateDailyCompact(const std::string& symbol)
{
    const Timeframe tf = Timeframe::Daily;

    auto lastTs = mStorage.getLastTimestamp(symbol, tf);

    MarketSeries marketSeries = mDownloader.download(symbol, tf);

    for (const auto& bar : marketSeries.bars) {
        if (lastTs && bar.ts <= *lastTs)
            continue;

        mStorage.insertBar(symbol, tf, bar);
    }
}