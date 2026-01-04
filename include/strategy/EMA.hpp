#pragma once

class Ema
{
public:
    explicit Ema(size_t period)
        : mAlpha(2.0 / (period + 1.0))
    {
    }

    void reset()
    {
        mInitialized = false;
    }

    double update(double price)
    {
        if(!mInitialized)
        {
            mValue = price;
            mInitialized = true;
            return mValue;
        }

        mValue = mAlpha * price + (1.0 - mAlpha) * mValue;
        return mValue;
    }

    bool isInitialized() const
    {
        return mInitialized;
    }

    double value() const
    {
        return mValue;
    }

private:
    double mAlpha;
    double mValue = 0.0;
    bool mInitialized = false;
};
