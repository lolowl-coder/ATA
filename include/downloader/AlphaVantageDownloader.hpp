#pragma once

#include "Downloader/Downloader.hpp"

class AlphaVantageDownloader : public Downloader {
public:
    explicit AlphaVantageDownloader(std::string apiKey);

    MarketSeries download(
        const std::string& symbol,
        Timeframe timeframe
    ) override;

private:
    std::string apiKey_;
};
