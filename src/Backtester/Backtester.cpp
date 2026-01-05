#include "backtester/Backtester.hpp"
#include "backtester/FixedFractionalAllocator.hpp"
#include "indicators/BasicIndicatorEngine.hpp"

void Backtester::run(
    const std::string& symbol,
    const MarketData& marketData,
    Strategy& strategy,
    CapitalAllocator& allocator,
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
                indicators.get({ IndicatorId::Volatility, static_cast<int>(endIndex - startIndex) }),
                i,
				portfolio_.positionQuantity(symbol) > 0.0
		    };
            StrategyDecision decision = strategy.evaluate(ctx, indicators);
            
            AllocationContext allocCtx{};
            allocCtx.capital = portfolio_.cash();
            allocCtx.price = ctx.closePrice;
            allocCtx.confidence = decision.confidence;
            allocCtx.openPositions = portfolio_.openPositionsCount();
            
            decision.quantity = allocator.allocate(allocCtx);
            
            if(decision.quantity <= 0.0)
            {
                continue;
            }

		    portfolio_.apply(symbol, decision, ctx.closePrice);

            OutputEventContext outputCtx {
                series.bars[i].ts,
                ctx,
                decision,
                portfolio_
		    };

            output_.onBar(outputCtx, priceSnapshot);
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

