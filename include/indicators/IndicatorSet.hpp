#pragma once

#include "Enums.hpp"

#include <unordered_map>

// IndicatorKey uniquely identifies a single indicator instance.
// period is the primary parameter (e.g. lookback window).
// period == 0 is allowed for non-windowed indicators
struct IndicatorKey {
    IndicatorId id;
    int period;   // or more later
    double param0;
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
    static IndicatorKey sma5 = { IndicatorId::SMA, 5 };
    static IndicatorKey sma10 = { IndicatorId::SMA, 10 };
    static IndicatorKey sma50 = { IndicatorId::SMA, 50 };
    static IndicatorKey ema5 = { IndicatorId::EMA, 5 };
    static IndicatorKey ema10 = { IndicatorId::EMA, 10 };
    static IndicatorKey rsi14 = { IndicatorId::RSI, 14 };
    static IndicatorKey volat10 = { IndicatorId::Volatility, 10 };
    static IndicatorKey volat14 = { IndicatorId::Volatility, 14 };
    static IndicatorKey volat20 = { IndicatorId::Volatility, 20 };
    static IndicatorKey volat30 = { IndicatorId::Volatility, 30 };
    static IndicatorKey volat60 = { IndicatorId::Volatility, 60 };
    static IndicatorKey volatPercentile50_50 = { IndicatorId::VolatilityPercentile, 20, 0.3 };
    static IndicatorKey atr14 = { IndicatorId::ATR, 14 };
    static IndicatorKey atr20 = { IndicatorId::ATR, 20 };
    static IndicatorKey atrPercentile14_40 = { IndicatorId::ATRPercentile, 14, 0.4 };
}

class IndicatorSet {
public:
	using Values = std::unordered_map<IndicatorKey, double, IndicatorKeyHash>;
    bool has(const IndicatorKey& key) const {
        return values_.contains(key);
    }

    double get(const IndicatorKey& key) const {
        // get() requires has() to be checked first.
        return has(key) ? values_.at(key) : 0.0;
    }

    void set(const IndicatorKey& key, double value) {
        values_[key] = value;
    }

    const Values & values() const {
        return values_;
	}

private:
    Values values_;
};
