#include "portfolio/Portfolio.hpp"

#include <stdexcept>

void Portfolio::trade(const std::string& symbol, double quantity, double price) {
    if (quantity == 0.0) return;

    double cost = quantity * price;
    if (quantity > 0 && cost > cash_) {
        throw std::runtime_error("Not enough cash");
    }

    Position& pos = positions_[symbol];

    if (quantity > 0) { // Buy
        pos.avgPrice = (pos.avgPrice * pos.quantity + price * quantity) / (pos.quantity + quantity);
        pos.quantity += quantity;
        cash_ -= cost;
    } else { // Sell
        if (-quantity > pos.quantity)
            throw std::runtime_error("Not enough shares to sell");
        pos.quantity += quantity;
        cash_ -= cost; // cost is negative
    }
}

const Position& Portfolio::getPosition(const std::string& symbol) const {
    static Position empty;
    auto it = positions_.find(symbol);
    return it != positions_.end() ? it->second : empty;
}

double Portfolio::equity(const PriceSnapshot& prices) const {
    double totalEquity = cash_;
    
    for (const auto& [symbol, pos] : positions_) {
        totalEquity += pos.quantity * prices.lastPrice.at(symbol); // Simplified: using avgPrice as current price
    }

    return totalEquity;
}

void Portfolio::apply(const std::string& symbol, const StrategyDecision& d, const double price)
{
    if (d.action == StrategyAction::Hold || d.quantity <= 0.0)
        return;

    auto& pos = positions_[symbol]; // creates empty position if not exists

    if (d.action == StrategyAction::Buy)
    {
        const double cost = d.quantity * price;

        // Optional: enforce cash constraint
        if (cash_ < cost)
        {
            throw std::runtime_error("Not enough cash to execute buy");
        }

        // Update avg entry price (weighted average)
        const double newQty = pos.quantity + d.quantity;
        pos.avgPrice =
            (pos.quantity * pos.avgPrice + d.quantity * price) / newQty;

        pos.quantity = newQty;
        cash_ -= cost;

        tradeCount_++;
    }
    else if (d.action == StrategyAction::Sell)
    {
		if (pos.quantity > 0.0)
        {
            const double sellQty = std::min(d.quantity, pos.quantity);

            // Realized PnL is based on avgEntryPrice
            realizedPnL_ += (price - pos.avgPrice) * sellQty;

            pos.quantity -= sellQty;
            cash_ += sellQty * price;

            tradeCount_++;

            // Remove empty position
            if (pos.quantity == 0.0)
            {
                positions_.erase(symbol);
            }
        }
    }
}
