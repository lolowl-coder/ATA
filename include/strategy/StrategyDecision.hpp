#pragma once

#include "Enums.hpp"

struct StrategyDecision {
    StrategyAction action;
    double confidence;   // 0..1 (optional, future-proof)
    double quantity;    // desired size (units or shares)
};
