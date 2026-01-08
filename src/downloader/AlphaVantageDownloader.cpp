#include "downloader/AlphaVantageDownloader.hpp"

inline std::string toAlphaVantageInterval(Timeframe tf)
{
    switch(tf) {
        case Timeframe::Minute5:  return "5min";
        case Timeframe::Minute15: return "15min";
        case Timeframe::Daily:    return "Daily";
    }
    throw std::logic_error("Unsupported timeframe");
}