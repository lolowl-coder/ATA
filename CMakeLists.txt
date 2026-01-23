cmake_minimum_required(VERSION 3.20)

project(ATA
    VERSION 0.1.0
    LANGUAGES C CXX
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

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

FetchContent_Declare(
    sqlite3
    URL https://www.sqlite.org/2026/sqlite-amalgamation-3510200.zip
)

FetchContent_MakeAvailable(sqlite3)

add_library(sqlite3 STATIC
    ${sqlite3_SOURCE_DIR}/sqlite3.c
)

target_compile_definitions(sqlite3
    PRIVATE
        SQLITE_THREADSAFE=1
        SQLITE_ENABLE_FTS5
        SQLITE_ENABLE_JSON1
)

set_target_properties(sqlite3 PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

target_include_directories(sqlite3 PUBLIC ${sqlite3_SOURCE_DIR})

FetchContent_Declare(
    curl
    GIT_REPOSITORY https://github.com/curl/curl.git
    GIT_TAG curl-8_18_0
)

set(BUILD_CURL_EXE OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
#set(CURL_USE_OPENSSL ON CACHE BOOL "" FORCE)
set(CURL_USE_SCHANNEL ON)
set(CURL_USE_OPENSSL OFF)
set(CURL_DISABLE_TESTS ON CACHE BOOL "" FORCE)
set(CURL_USE_LIBPSL OFF CACHE BOOL "" FORCE)


FetchContent_MakeAvailable(curl)

# ---- Library target (headers only for now) ----------------------------

set(headers
	"include/backtester/Backtester.hpp"
	"include/backtester/CapitalAllocator.hpp"
	"include/backtester/FixedFractionalAllocator.hpp"
	"include/core/MarketData.hpp"
	"include/core/Types.hpp"
	"include/downloader/AlphaVantageDownloader.hpp"
	"include/downloader/AlphaVantageParser.hpp"
	"include/downloader/Downloader.hpp"
	"include/downloader/CurlHttpClient.hpp"
	"include/downloader/IHttpClient.hpp"
	"include/indicators/BasicIndicatorEngine.hpp"
	"include/indicators/IndicatorEngine.hpp"
	"include/indicators/Indicators.hpp"
	"include/indicators/IndicatorSet.hpp"
	"include/output/ConsoleOutput.hpp"
	"include/output/Output.hpp"
	"include/portfolio/Portfolio.hpp"
	"include/storage/MarketDataUpdater.hpp"
	"include/storage/SQLiteStorage.hpp"
	"include/strategy/CrossoverStrategy.hpp"
	"include/strategy/MovingAverageStrategy.hpp"
	"include/strategy/Strategy.hpp"
	"include/strategy/StrategyContext.hpp"
	"include/strategy/StrategyDecision.hpp"
	"include/utils/TimeUtils.hpp"
	"include/Enums.hpp"
	)

set(sources
	"src/backtester/Backtester.cpp"
	"src/downloader/AlphaVantageDownloader.cpp"
	"src/downloader/AlphaVantageParser.cpp"
	"src/downloader/CurlHttpClient.cpp"
	"src/indicators/BasicIndicatorEngine.cpp"
	"src/indicators/Indicators.cpp"
	"src/output/ConsoleOutput.cpp"
	"src/strategy/CrossoverStrategy.cpp"
	"src/strategy/MovingAverageStrategy.cpp"
	"src/storage/MarketDataUpdater.cpp"
	"src/storage/SQLiteStorage.cpp"
	"src/portfolio/Portfolio.cpp"
	"src/utils/TimeUtils.cpp"
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

target_link_libraries(ATA
	PRIVATE
		nlohmann_json::nlohmann_json
		sqlite3
		libcurl
)

add_executable(Backtest src/main.cpp)
target_include_directories(Backtest
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(Backtest
	PRIVATE
		ATA
)

set_property(TARGET Backtest PROPERTY VS_STARTUP_PROJECT TRUE)

# ---- Testing ----------------------------------------------------------

include(CTest)
enable_testing()

add_subdirectory(tests)
