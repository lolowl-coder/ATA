#include "output/ConsoleOutput.hpp"

#include <iostream>
#include <iomanip>

void ConsoleOutput::onBar(const OutputEventContext& ctx, const PriceSnapshot& priceSnapshot)
{
    // Minimal heartbeat (can be disabled later)
    std::cout
        << "[BAR] "
        << " Symbol " << ctx.symbol
        << " Time " << ctx.ts
        << " Close " << ctx.strategyCtx.closePrice
        << " Volatility " << ctx.strategyCtx.volatility
        << " Decision " << strategyActionToString(ctx.decision.action)
        << " Confidence " << ctx.decision.confidence
        << " Quantity " << ctx.decision.quantity
        << " Equity " << ctx.portfolio.equity(priceSnapshot)
        << " Cash " << ctx.portfolio.cash()
        << "\n";
}

void ConsoleOutput::onTrade(const OutputEventContext& ctx, const PriceSnapshot& priceSnapshot)
{
    const auto& d = ctx.decision;

    std::cout
        << "[TRADE] "
        << ctx.ts
        << " "
        << strategyActionToString(d.action)
        << " Qty " << d.quantity
        << " Price " << ctx.strategyCtx.closePrice
        << " Confidence " << std::fixed << std::setprecision(2) << d.confidence
        << " Equity " << ctx.portfolio.equity(priceSnapshot)
        << "\n";
}

void ConsoleOutput::onFinish(const Portfolio& portfolio, const PriceSnapshot& priceSnapshot)
{
    std::cout << "\n===== BACKTEST SUMMARY =====\n";
    std::cout << "Final Equity: " << portfolio.equity(priceSnapshot) << "\n";
    std::cout << "Total PnL:    " << portfolio.realizedPnL() << "\n";
    std::cout << "Trades:       " << portfolio.tradeCount() << "\n";
}
