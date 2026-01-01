#pragma once

#include "backtester/CapitalAllocator.hpp"

class FixedFractionalAllocator final : public CapitalAllocator
{
public:
    FixedFractionalAllocator(
        double riskFraction,
        size_t maxPositions)
        : mRiskFraction(riskFraction)
        , mMaxPositions(maxPositions)
    {
    }

    double allocate(const AllocationContext& ctx) const override
    {
        if(ctx.price <= 0.0)
        {
            return 0.0;
        }

        if(ctx.capital <= 0.0)
        {
            return 0.0;
        }

        if(ctx.confidence <= 0.0)
        {
            return 0.0;
        }

        if(ctx.openPositions >= mMaxPositions)
        {
            return 0.0;
        }

        const double positionFraction =
            mRiskFraction / static_cast<double>(mMaxPositions);

        const double allocatedCapital =
            ctx.capital * positionFraction * ctx.confidence;

        if(allocatedCapital <= 0.0)
        {
            return 0.0;
        }

        return allocatedCapital / ctx.price;
    }

private:
    double mRiskFraction;
    size_t mMaxPositions;
};
