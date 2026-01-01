#pragma once

#include "portfolio/Portfolio.hpp"
#include "strategy/StrategyDecision.hpp"

#include <string>

struct AllocationContext
{
    double capital;
    double price;
    double confidence;
    size_t openPositions;
};

class CapitalAllocator
{
public:
    virtual ~CapitalAllocator() = default;

    virtual double allocate(const AllocationContext& ctx) const = 0;
};
