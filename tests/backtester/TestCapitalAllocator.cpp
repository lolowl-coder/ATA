#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "backtester/FixedFractionalAllocator.hpp"

#include <doctest/doctest.h>

TEST_CASE("FixedFractionalAllocator returns zero on invalid inputs")
{
    FixedFractionalAllocator allocator(0.2, 5);

    AllocationContext ctx{};
    ctx.capital = 100000.0;
    ctx.price = 0.0;
    ctx.confidence = 1.0;
    ctx.openPositions = 0;

    CHECK(allocator.allocate(ctx) == 0.0);
}