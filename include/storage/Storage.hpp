#pragma once

#include "core/MarketData.hpp"
#include "core/Types.hpp"
#include "Enums.hpp"

#incldue <string>

class Storage {
public:
    virtual ~Storage() = default;

    virtual void save(const MarketSeries& series) = 0;

    virtual MarketSeries load(
        const std::string& symbol,
        Timeframe timeframe
    ) = 0;

    virtual TimePoint lastUpdate(
        const std::string& symbol,
        Timeframe timeframe
    ) = 0;
};
