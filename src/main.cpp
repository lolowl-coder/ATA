#include "backtester/Backtester.hpp"
#include "backtester/FixedFractionalAllocator.hpp"
#include "core/MarketData.hpp"
#include "downloader/CurlHttpClient.hpp"
#include "downloader/AlphaVantageDownloader.hpp"
#include "indicators/BasicIndicatorEngine.hpp"
#include "output/ConsoleOutput.hpp"
#include "portfolio/Portfolio.hpp"
#include "strategy/MovingAverageStrategy.hpp"
#include "strategy/CrossoverStrategy.hpp"
#include "storage/SQLiteStorage.hpp"

#include <memory>
#include <iostream>

void updateMarketData(
    SQLiteStorage& storage,
    const std::string& symbol,
    Timeframe timeframe
)
{
    const std::string apiKey = "MZMDA03UMY6CC4DQ";
    auto http = std::make_unique<CurlHttpClient>();
    AlphaVantageDownloader dl(
        apiKey,
        std::move(http)
    );
    MarketSeries series = dl.download(symbol, timeframe);
    storage.store(series);
}

int main()
{
	Portfolio portfolio(10000.0);
    BasicIndicatorEngine engine;
    ConsoleOutput output;
    Backtester bt(portfolio, engine, output);
    SQLiteStorage storage("at.sqlite");
    
    std::vector<std::string> symbols =
    {
        "AMZN",
        "CRWD",
        "ENB",
        "KEY",
        "KILO-B.TRT",
        "LAKE",
        "TSLA",
        "NVDA",
        "PLTR"
    };
	const Timeframe timeframe = Timeframe::Daily;

    const float risk = 0.4f; // 20% risk per trade
    const size_t maxPositions = 10;
    FixedFractionalAllocator capitalAllocator(risk, maxPositions);

	for(auto& symbol : symbols)
    {
		std::cout << "Running backtest for " << symbol << "...\n";
        //if(storage.getLastTimestamp(symbol, timeframe) )
        if(symbol == "")
        {
            updateMarketData(
                storage,
                symbol,
                timeframe
            );
        }

        MarketSeries series = storage.load(symbol, timeframe);
        if(!series.bars.empty())
        {
            MarketData marketData = { series };
            engine.resetVolatilityHistory();
            engine.resetATRHistory();
            CrossoverStrategy strategy(
                indicators::sma5,
                indicators::sma10,
                indicators::volat14,
                indicators::volat20,
                indicators::volatPercentile50_50,
                indicators::atr14,
                indicators::atrPercentile14_40
                );
			//MovingAverageStrategy strategy;

            bt.run(symbol, marketData, strategy, capitalAllocator, strategy.getMaxPeriod() - 1, series.bars.size() - 1);
        }
    }
	
	return 0;
}