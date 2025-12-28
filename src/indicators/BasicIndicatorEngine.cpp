#include "indicators/BasicIndicatorEngine.hpp"

IndicatorSet BasicIndicatorEngine::compute(
    const MarketSeries& series,
    size_t index,
    const std::vector<IndicatorKey>& required
) const
{
    IndicatorSet set;

    std::vector<double> closes;
    closes.reserve(index + 1);
    for (size_t i = 0; i <= index; ++i) {
        closes.push_back(series.bars[i].close);
    }

    for (auto k : required) {
        switch (k.id) {
        case IndicatorId::SMA:
            set.set(k, Indicators::sma(closes, k.period));
            break;
        case IndicatorId::RSI:
            set.set(k, Indicators::rsi(closes, k.period));
            break;
        case IndicatorId::Volatility:
            set.set(k, Indicators::volatility(closes, k.period));
            break;
        }
    }

    return set;
}
