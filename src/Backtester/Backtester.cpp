#include "backtester/Backtester.hpp"
#include "indicators/BasicIndicatorEngine.hpp"

void Backtester::run(
    const std::string& symbol,
    const MarketData& marketData,
    Strategy& strategy,
    const size_t startIndex,
    const size_t endIndex
) {
    const auto required = strategy.requiredIndicators();

    for (size_t i = startIndex; i <= endIndex; ++i)
    {
        PriceSnapshot priceSnapshot;
        // 1. Build snapshot
        for (const auto& series : marketData)
        {
            priceSnapshot.prices[series.symbol] = series.bars[i].close;
        }

        // 2. Run strategy
        for (const auto& series : marketData)
        {
            IndicatorSet indicators = indicatorEngine_.compute(series, i, required);
            StrategyContext ctx{
                series.bars[i].close,
                i
		    };
            StrategyDecision decision = strategy.evaluate(ctx, indicators);

            const double price = series.bars[i].close;

            OutputEventContext outputCtx {
                series.bars[i].ts,
                ctx,
                decision,
                portfolio_
		    };
            output_.onBar(outputCtx, priceSnapshot);

		    portfolio_.apply(symbol, decision, price);

        }
    }
  
    // Rebuild snapshot with latest prices
    PriceSnapshot priceSnapshot;
    for (const auto& series : marketData)
    {
        priceSnapshot.prices[series.symbol] = series.bars.back().close;
    }
    output_.onFinish(portfolio_, priceSnapshot);
}

