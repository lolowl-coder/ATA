#pragma once

#include <vector>
#include <algorithm>
#include <stdexcept>

bool isZero(double x, double tol = 1e-9);

double percentile(
    const std::vector<double>& values,
    const size_t window,
    const double p);