#pragma once

#include "Enums.hpp"

struct StrategyDecision {
    StrategyAction action = StrategyAction::Hold;
    double confidence = 0.0;   // 0..1 (optional, future-proof)
    double quantity = 0.0;    // desired size (units or shares)
};
