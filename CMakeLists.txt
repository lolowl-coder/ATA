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
	"include/Backtester/Backtester.hpp"
	"include/Core/MarketData.hpp"
	"include/Core/Types.hpp"
	"include/Downloader/AlphaVantageDownloader.hpp"
	"include/Downloader/Downloader.hpp"
	"include/Indicators/BasicIndicatorEngine.hpp"
	"include/Indicators/IndicatorEngine.hpp"
	"include/Indicators/Indicators.hpp"
	"include/Indicators/IndicatorSet.hpp"
	"include/Output/ConsoleOutput.hpp"
	"include/Output/Output.hpp"
	"include/Portfolio/Portfolio.hpp"
	"include/Storage/SQLiteStorage.hpp"
	"include/Storage/Storage.hpp"
	"include/Strategy/MovingAverageStrategy.hpp"
	"include/Strategy/Strategy.hpp"
	"include/Strategy/StrategyContext.hpp"
	"include/Strategy/StrategyDecision.hpp"
	"include/Enums.hpp"
	)

set(sources
	"src/backtester/Backtester.cpp"
	"src/indicators/BasicIndicatorEngine.cpp"
	"src/indicators/Indicators.cpp"
	"src/output/ConsoleOutput.cpp"
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
