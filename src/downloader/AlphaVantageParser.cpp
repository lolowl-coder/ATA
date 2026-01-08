#include "downloader/AlphaVantageParser.hpp"
#include "TimeUtils.hpp"

#include <nlohmann/json.hpp>
#include <algorithm>

using json = nlohmann::json;

namespace {

    std::string timeSeriesKey(Timeframe tf)
    {
        switch(tf) {
            case Timeframe::Daily:    return "Time Series (Daily)";
            case Timeframe::Minute5:  return "Time Series (5min)";
            case Timeframe::Minute15: return "Time Series (15min)";
        }
        throw std::logic_error("Unsupported timeframe");
    }

    double getDouble(const json& j, const char* key)
    {
        return std::stod(j.at(key).get<std::string>());
    }

}

MarketSeries AlphaVantageParser::parse(
    const std::string& jsonText,
    const std::string& symbol,
    Timeframe timeframe
)
{
    const json root = json::parse(jsonText);

    const auto tsKey = timeSeriesKey(timeframe);
    if(!root.contains(tsKey)) {
        throw std::runtime_error("Missing time series key");
    }

    const auto& ts = root.at(tsKey);

    Bars bars;
    bars.reserve(ts.size());

    for(const auto& [timestamp, values] : ts.items()) {
        Bar bar;
        bar.ts     = parseNewYorkToUtc(timestamp);
        bar.open   = getDouble(values, "1. open");
        bar.high   = getDouble(values, "2. high");
        bar.low    = getDouble(values, "3. low");
        bar.close  = getDouble(values, "4. close");
        bar.volume = getDouble(values, "5. volume");

        bars.push_back(bar);
    }

    std::sort(
        bars.begin(),
        bars.end(),
        [](const Bar& a, const Bar& b) {
            return a.ts < b.ts;
        }
    );

    if(bars.empty()) {
        throw std::runtime_error("No bars parsed");
    }

    return MarketSeries{
        symbol,
        timeframe,
        std::move(bars)
    };
}
