#pragma once

#include <string>
#include <optional>

#include "core/MarketData.hpp"

class SQLiteStorage {
public:
    explicit SQLiteStorage(const std::string& dbPath);
    void setupPragmas();
    ~SQLiteStorage();

    void store(const MarketSeries& series);

    MarketSeries load(
        const std::string& symbol,
        Timeframe timeframe
    ) const;

    std::optional<TimePoint> getLastTimestamp(
        const std::string& symbol,
        Timeframe timeframe
    ) const;

    void insertBar(const std::string& symbol,
                   Timeframe tf,
                   const Bar& bar);

    size_t countBars(const std::string& symbol, Timeframe tf) const;

private:
    void open(const std::string& path);
    void initSchema();

    void exec(const std::string& sql) const;

private:
    void* mDb = nullptr; // sqlite3*
};
