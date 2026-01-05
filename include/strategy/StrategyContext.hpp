#pragma once

struct StrategyContext {
    double closePrice;
    double volatility;
    size_t index;
    bool hasPosition;
};
