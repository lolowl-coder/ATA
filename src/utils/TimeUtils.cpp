#include "utils/TimeUtils.hpp"

#include <sstream>
#include <iomanip>
#include <ctime>

TimePoint parseNewYorkToUtc(const std::string& timestamp)
{
    std::tm tm{};
    std::istringstream ss(timestamp);

    if(timestamp.size() > 10) {
        ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    } else {
        ss >> std::get_time(&tm, "%Y-%m-%d");
    }

    if(ss.fail()) {
        throw std::runtime_error("Failed to parse timestamp");
    }

    // Treat parsed time as New York local time
    tm.tm_isdst = -1;

    std::time_t local = std::mktime(&tm);
    if(local == -1) {
        throw std::runtime_error("Invalid time");
    }

    // Convert to UTC
    std::tm* gmt = std::gmtime(&local);
    return std::chrono::system_clock::from_time_t(std::mktime(gmt));
}
