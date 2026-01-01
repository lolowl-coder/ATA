#pragma once

#include "strategy/StrategyDecision.hpp"

#include <string>
#include <unordered_map>

// Snapshot of last prices per symbol
struct PriceSnapshot {
    std::unordered_map<std::string, double> prices;
};

// Single position for a symbol
struct Position {
    double quantity = 0.0;
    double avgPrice = 0.0;
};

class Portfolio {
public:
    Portfolio(double initialCash) : cash_(initialCash) {}

    // Buy or sell symbol
    void trade(const std::string& symbol, double quantity, double price);

    // Get current position
    const Position& getPosition(const std::string& symbol) const;

    // Cash balance
    double cash() const { return cash_; }

	// Total equity (cash + market value of positions)
	double equity(const PriceSnapshot& priceSnapshot) const;

	// Apply strategy decision
    void apply(const std::string& symbol, const StrategyDecision& d, const double price);

    double realizedPnL() const { return realizedPnL_; }

    size_t tradeCount() const { return tradeCount_; }

    double positionQuantity(const std::string& symbol) const {
        auto it = positions_.find(symbol);
        if (it != positions_.end()) {
            return it->second.quantity;
        }
        return 0.0;
	}

    size_t openPositionsCount() const {
        return positions_.size();
	}

private:
	// Cash balance
    double cash_ = 0.0;
	// Holdings per symbol
    std::unordered_map<std::string, Position> positions_;
	// Realized profit and loss
    double realizedPnL_ = 0.0;
	// Number of trades executed
    size_t tradeCount_ = 0;
};
