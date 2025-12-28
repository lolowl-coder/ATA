#pragma once

#include "core/MarketData.hpp"
#include "core/Types.hpp"

#include <string>

class Downloader {
public:
    virtual ~Downloader() = default;

    virtual MarketSeries download(
        const std::string& symbol,
        Timeframe timeframe
    ) = 0;
};
