#pragma once

struct StrategyContext {
    double closePrice;
    double volatilituy;
    size_t index;
    bool hasPosition;
};
