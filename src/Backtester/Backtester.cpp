#include "backtester/Backtester.hpp"
#include "indicators/BasicIndicatorEngine.hpp"

void Backtester::run(
    const std::string& symbol,
    const MarketSeries& series,
    Strategy& strategy
) {
    const auto required = strategy.requiredIndicators();

    for (size_t i = 0; i < series.bars.size(); ++i) {
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

    output_.onFinish(portfolio_);
}

