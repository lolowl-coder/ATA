#include "downloader/AlphaVantageDownloader.hpp"
#include "downloader/AlphaVantageParser.hpp"

#include <stdexcept>

namespace {

std::string intervalString(Timeframe tf)
{
    switch(tf) {
        case Timeframe::Daily:    return "";
        case Timeframe::Minute5:  return "5min";
        case Timeframe::Minute15: return "15min";
    }
    throw std::logic_error("Unsupported timeframe");
}

}

AlphaVantageDownloader::AlphaVantageDownloader(
    std::string apiKey,
    std::unique_ptr<IHttpClient> http
)
    : mApiKey(std::move(apiKey))
    , mHttp(std::move(http))
{
}

MarketSeries AlphaVantageDownloader::download(
    const std::string& symbol,
    Timeframe timeframe
) const
{
    const auto url = buildUrl(symbol, timeframe);
    const auto json = mHttp->get(url);

    return AlphaVantageParser::parse(
        json,
        symbol,
        timeframe
    );
}

std::string AlphaVantageDownloader::buildUrl(
    const std::string& symbol,
    Timeframe timeframe
) const
{
    if(timeframe == Timeframe::Daily) {
        return
            "https://www.alphavantage.co/query"
            "?function=TIME_SERIES_DAILY"
            "&symbol=" + symbol +
            "&apikey=" + mApiKey +
            "&outputsize=compact";
    }

    return
        "https://www.alphavantage.co/query"
        "?function=TIME_SERIES_INTRADAY"
        "&symbol=" + symbol +
        "&interval=" + intervalString(timeframe) +
        "&apikey=" + mApiKey +
        "&outputsize=compact" +
        "&adjusted=true";
}
