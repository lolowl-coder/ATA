#pragma once

#include <deque>

class RollingVolatility
{
public:
    explicit RollingVolatility(size_t window)
        : mWindow(window)
    {
    }

    void reset()
    {
        mValues.clear();
        mSum = 0.0;
        mSumSq = 0.0;
    }

    void update(double price)
    {
        mValues.push_back(price);
        mSum += price;
        mSumSq += price * price;

        if(mValues.size() > mWindow)
        {
            double old = mValues.front();
            mValues.pop_front();
            mSum -= old;
            mSumSq -= old * old;
        }
    }

    bool isReady() const
    {
        return mValues.size() == mWindow;
    }

    double value() const
    {
        size_t n = mValues.size();
        if(n == 0)
            return 0.0;

        double mean = mSum / n;
        double variance = (mSumSq / n) - (mean * mean);

        return variance > 0.0 ? std::sqrt(variance) : 0.0;
    }

private:
    size_t mWindow;
    std::deque<double> mValues;
    double mSum = 0.0;
    double mSumSq = 0.0;
};
