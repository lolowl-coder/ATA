#include "indicators/BasicIndicatorEngine.hpp"

IndicatorSet BasicIndicatorEngine::compute(
    const MarketSeries& series,
    size_t index,
    const std::vector<IndicatorKey>& required
)
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
        case IndicatorId::EMA:
            set.set(k, Indicators::ema(closes, k.period));
            break;
        case IndicatorId::RSI:
            set.set(k, Indicators::rsi(closes, k.period));
            break;
        case IndicatorId::Volatility:
            {
                auto volat = Indicators::volatility(closes, k.period);
                set.set(k, volat);
                mVolatilityHistory.push_back(volat);
            }
            break;
        case IndicatorId::VolatilityPercentile:
            set.set(k, Indicators::volatilityPercentile(mVolatilityHistory, k.period, k.param0));
            break;
        case IndicatorId::ATR:
            {
			    auto atr = Indicators::ATR(series.bars, k.period);
                set.set(k, atr);
				mATRHistory.push_back(atr);
            }
            break;
        case IndicatorId::ATRPercentile:
            {
			    auto atr = Indicators::ATRPercentile(mATRHistory, k.period, k.param0);
                set.set(k, atr);
				mATRHistory.push_back(atr);
            }
            break;
        default:
            throw std::runtime_error("Indicator not implementd");
		    break;
        }
    }

    return set;
}
