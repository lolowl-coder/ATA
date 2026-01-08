#pragma once

#include "downloader/Downloader.hpp"

class AlphaVantageDownloader final : public Downloader {
public:
    explicit AlphaVantageDownloader(std::string apiKey);

    MarketSeries download(
        const std::string& symbol,
        Timeframe timeframe
    ) const override;

private:
    std::string mApiKey;
};