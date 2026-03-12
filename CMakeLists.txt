cmake_minimum_required(VERSION 3.14)
project(sea_battle_game)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build tests" ON)

add_executable(sea_battle_game
    main.cpp
)

target_include_directories(sea_battle_game PRIVATE include)

if(BUILD_TESTS)
    enable_testing()

    include(FetchContent)

    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    add_executable(sea_battle_tests
        tests/test_main.cpp
        tests/test_ships.cpp
        tests/test_fleet.cpp
        tests/test_board.cpp
        tests/test_player.cpp
        tests/test_game.cpp
    )

    target_include_directories(sea_battle_tests PRIVATE include)

    target_link_libraries(sea_battle_tests
        GTest::gtest_main
    )

    include(GoogleTest)
    gtest_discover_tests(sea_battle_tests)
endif()