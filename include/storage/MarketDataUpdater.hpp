#pragma once

#include "core/Types.hpp"
#include "downloader/AlphaVantageDownloader.hpp"
#include "storage/SQLiteStorage.hpp"

#include <vector>
#include <string>
#include <chrono>

class MarketDataUpdater {
public:
    MarketDataUpdater(
        AlphaVantageDownloader& downloader,
        SQLiteStorage& storage
    );

    void updateDailyCompact(const std::string& symbol);

private:
    bool needsUpdate(
        const std::string& symbol,
        Timeframe timeframe,
        const std::chrono::system_clock::time_point& nowUtc
    ) const;

private:
    AlphaVantageDownloader& mDownloader;
    SQLiteStorage& mStorage;
};
