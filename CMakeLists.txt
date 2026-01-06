cmake_minimum_required(VERSION 3.20)

project(ATA
    VERSION 0.1.0
    LANGUAGES CXX
)

# ---- Global settings -------------------------------------------------

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Good warnings by default
if (MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# ---- Library target (headers only for now) ----------------------------

set(headers
	"include/backtester/Backtester.hpp"
	"include/backtester/CapitalAllocator.hpp"
	"include/backtester/FixedFractionalAllocator.hpp"
	"include/core/MarketData.hpp"
	"include/core/Types.hpp"
	"include/downloader/AlphaVantageDownloader.hpp"
	"include/downloader/Downloader.hpp"
	"include/indicators/BasicIndicatorEngine.hpp"
	"include/indicators/IndicatorEngine.hpp"
	"include/indicators/Indicators.hpp"
	"include/indicators/IndicatorSet.hpp"
	"include/output/ConsoleOutput.hpp"
	"include/output/Output.hpp"
	"include/portfolio/Portfolio.hpp"
	"include/storage/SQLiteStorage.hpp"
	"include/storage/Storage.hpp"
	"include/strategy/CrossoverStrategy.hpp"
	"include/strategy/MovingAverageStrategy.hpp"
	"include/strategy/Strategy.hpp"
	"include/strategy/StrategyContext.hpp"
	"include/strategy/StrategyDecision.hpp"
	"include/Enums.hpp"
	)

set(sources
	"src/backtester/Backtester.cpp"
	
	"src/indicators/BasicIndicatorEngine.cpp"
	"src/indicators/Indicators.cpp"
	"src/output/ConsoleOutput.cpp"
	"src/strategy/CrossoverStrategy.cpp"
	"src/strategy/MovingAverageStrategy.cpp"
	"src/portfolio/Portfolio.cpp"
	)

add_library(ATA)

target_sources(ATA
	PRIVATE
		${sources}
	PUBLIC
		${headers}
	)

target_include_directories(ATA
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ---- Testing ----------------------------------------------------------

include(CTest)
enable_testing()

add_subdirectory(tests)
