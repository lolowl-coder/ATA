#include "backtester/Backtester.hpp"
#include "backtester/FixedFractionalAllocator.hpp"
#include "core/MarketData.hpp"
#include "downloader/CurlHttpClient.hpp"
#include "downloader/AlphaVantageDownloader.hpp"
#include "indicators/BasicIndicatorEngine.hpp"
#include "output/ConsoleOutput.hpp"
#include "portfolio/Portfolio.hpp"
#include "strategy/MovingAverageStrategy.hpp"
#include "storage/SQLiteStorage.hpp"

#include <memory>

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
    
    const std::string symbol = "NVDA";
	const Timeframe timeframe = Timeframe::Daily;

    /*updateMarketData(
        storage,
        symbol,
        timeframe
	);*/

    MarketSeries series = storage.load(symbol, timeframe);
    MarketData marketData = { series };

    MovingAverageStrategy maStrategy;

	const float risk = 0.2f; // 20% risk per trade
	const size_t maxPositions = 4;
	FixedFractionalAllocator capitalAllocator(risk, maxPositions);

    bt.run(symbol, marketData, maStrategy, capitalAllocator, 4, series.bars.size() - 1);
	
	return 0;
}