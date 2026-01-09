#pragma once

#include "core/MarketData.hpp"
#include "downloader/IHttpClient.hpp"
#include "Enums.hpp"

#include <memory>
#include <string>

class AlphaVantageDownloader {
public:
    AlphaVantageDownloader(
        std::string apiKey,
        std::unique_ptr<IHttpClient> http
    );

    MarketSeries download(
        const std::string& symbol,
        Timeframe timeframe
    ) const;

private:
    std::string mApiKey;
    std::unique_ptr<IHttpClient> mHttp;

    std::string buildUrl(
        const std::string& symbol,
        Timeframe timeframe
    ) const;
};
