include(FetchContent)

# ---- Fetch doctest ----------------------------------------------------

FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG v2.4.11
)

FetchContent_MakeAvailable(doctest)

function(addTest exeName sources)
    add_executable(
        ${exeName}
        ${sources}
        )

    target_sources(
        ${exeName}
	    PRIVATE
		    ${sources}
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

set(
    testSources
    )
# Indicators
addTest(
    TestIndicators
    "indicators/TestIndicators.cpp"
    )
# Strategies
addTest(
    TestStrategies
    "strategy/TestMovingAverage.cpp"
    "strategy/TestMovingAverageCrossover.cpp"
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
