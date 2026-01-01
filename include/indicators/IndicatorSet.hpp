#pragma once

#include <unordered_map>

// IndicatorKey uniquely identifies a single indicator instance.
// period is the primary parameter (e.g. lookback window).
// period == 0 is allowed for non-windowed indicators
struct IndicatorKey {
    IndicatorId id;
    int period;   // or more later
    double value;

    bool operator==(const IndicatorKey& other) const noexcept {
        return id == other.id && period == other.period && std::abs(value - other.value) < std::numeric_limits<double>::epsilon();
    }
};

struct IndicatorKeyHash {
    size_t operator()(const IndicatorKey& k) const noexcept {
        size_t h1 = std::hash<int>{}(static_cast<int>(k.id));
        size_t h2 = std::hash<int>{}(k.period);
        return h1 * 31u + h2; // or boost-style combine
    }
};

namespace indicators
{
    static IndicatorKey smaFast = { IndicatorId::SMA, 5 };
    static IndicatorKey smaSlow = { IndicatorId::SMA, 10 };
    static IndicatorKey rsi14 = { IndicatorId::RSI, 14 };
    static IndicatorKey volat14 = { IndicatorId::Volatility, 14 };
    static IndicatorKey volat20 = { IndicatorId::Volatility, 20 };
}

class IndicatorSet {
public:
    bool has(const IndicatorKey& key) const {
        return values_.contains(key);
    }

    double get(const IndicatorKey& key) const {
        // get() requires has() to be checked first.
        return values_.at(key);
    }

    void set(const IndicatorKey& key, double value) {
        values_[key] = value;
    }

private:
    std::unordered_map<IndicatorKey, double, IndicatorKeyHash> values_;
};
