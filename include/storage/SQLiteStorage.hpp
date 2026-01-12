#pragma once

#include <string>
#include <optional>

#include "core/MarketData.hpp"

class SQLiteStorage {
public:
    explicit SQLiteStorage(const std::string& dbPath);
    ~SQLiteStorage();

    void store(const MarketSeries& series);

    MarketSeries load(
        const std::string& symbol,
        Timeframe timeframe
    ) const;

    std::optional<TimePoint> lastTimestamp(
        const std::string& symbol,
        Timeframe timeframe
    ) const;

private:
    void open(const std::string& path);
    void initSchema();

    void exec(const std::string& sql) const;

private:
    void* mDb = nullptr; // sqlite3*
};
