set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# ---- Fetch doctest ----------------------------------------------------

FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG v2.4.11
)

FetchContent_MakeAvailable(doctest)

function(addTest exeName)
    add_executable(
        ${exeName}
        ${ARGN}
        )

    target_sources(
        ${exeName}
	    PRIVATE
		    ${ARGN}
	    )

    target_link_libraries(
        ${exeName}
        PRIVATE
            ATA
            doctest::doctest
        )

    add_test(
        NAME ${exeName}
        COMMAND ${exeName}
        )

endfunction()

# Indicators
addTest(
    TestIndicators
    "indicators/TestIndicators.cpp"
    )
# Strategies
addTest(
    TestMovingAverage
    "strategy/TestMovingAverage.cpp"
    )
addTest(
    TestCrossover
    "strategy/TestCrossover.cpp"
    )
# Backtester
addTest(
    TestBacktester
    "backtester/TestBacktester.cpp"
    )
# Capital Allocator
addTest(
    TestCapitalAllocator
    "backtester/TestCapitalAllocator.cpp"
    )
# Downloader
addTest(
    TestAlphaVantageParser
    "downloader/TestAlphaVantageParser.cpp"
    )
addTest(
    TestAlphaVantageDownloader
    "downloader/TestAlphaVantageDownloader.cpp"
    )
