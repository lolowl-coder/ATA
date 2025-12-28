#pragma once

#include "Output/Output.hpp"
#include "Portfolio/Portfolio.hpp"

class ConsoleOutput : public Output
{
public:
    void onBar(const OutputEventContext& ctx, const PriceSnapshot& priceSnapshot) override;
    void onTrade(const OutputEventContext& ctx, const PriceSnapshot& priceSnapshot) override;
    void onFinish(const Portfolio& portfolio, const PriceSnapshot& priceSnapshot) override;
};
