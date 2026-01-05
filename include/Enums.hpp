#pragma once

#define TIME_FRAME \
	X(Daily) \
	X(Minute5) \
	X(Minute15)

enum class Timeframe {
	#define X(value) value,
	TIME_FRAME
	#undef X
};

static const char* timeFrameToString(Timeframe value)
{
	switch (value) {
		#define X(value) case Timeframe::value: return #value;
		TIME_FRAME
		#undef X
		default:
			return "Unknown Time frame";
	}
}

#define STRATEGY_ACTION \
	X(Buy) \
	X(Sell) \
	X(Hold)

enum class StrategyAction {
	#define X(value) value,
	STRATEGY_ACTION
	#undef X
};

static const char* strategyActionToString(StrategyAction value)
{
	switch (value) {
		#define X(value) case StrategyAction::value: return #value;
		STRATEGY_ACTION
		#undef X
		default:
			return "Unknown Strategy Action";
	}
}

#define INDICATOR_ID \
	X(SMA) \
	X(EMA) \
	X(RSI) \
	X(Volatility)

enum class IndicatorId {
	#define X(value) value,
	INDICATOR_ID
	#undef X
};

static const char* indicatorIdToString(IndicatorId value)
{
	switch (value) {
		#define X(value) case IndicatorId::value: return #value;
		INDICATOR_ID
		#undef X
		default:
			return "Unknown Indicator Id";
	}
}
