#pragma once

#include "storage/Storage.hpp"
#include "core/MarketData.hpp"
#include "core/Types.hpp"
#include "Enums.hpp"

#include <string>

class SQLiteStorage : public Storage {
public:
    explicit SQLiteStorage(const std::string& dbPath);

    void save(const MarketSeries& series) override;
    MarketSeries load(const std::string& symbol, Timeframe timeframe) override;
    TimePoint lastUpdate(const std::string& symbol, Timeframe timeframe) override;

private:
    std::string dbPath_;
};
