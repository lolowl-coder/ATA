#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "core/Math.hpp"

#include <doctest/doctest.h>

TEST_CASE("percentile basic")
{
    std::vector<double> v = { 0, 1, 2, 3, 4, 5, 6 };

    CHECK(percentile(v, 5, 0.0) == 2);
    CHECK(percentile(v, 5, 0.5) == 4);
    CHECK(percentile(v, 5, 1.0) == 6);
}
