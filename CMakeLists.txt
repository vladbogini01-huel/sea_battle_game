cmake_minimum_required(VERSION 3.14)
project(sea_battle_game)

## Устанавливаем политику CMP0135 для подавления предупреждения
#if(POLICY CMP0135)
#    cmake_policy(SET CMP0135 NEW)
#endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## Опция для включения тестов
#option(BUILD_TESTS "Build tests" ON)

# Основной исполняемый файл
add_executable(sea_battle_game main.cpp
        include/fleet.hpp
        include/ships.hpp
        include/constants.hpp
        include/player.hpp
        include/game.hpp
        include/board.hpp
)

## Тесты
#if(BUILD_TESTS)
#    # Включаем CMake-функции для Google Test
#    include(FetchContent)
#
#    # Скачиваем Google Test с указанием DOWNLOAD_EXTRACT_TIMESTAMP
#    FetchContent_Declare(
#            googletest
#            URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
#            DOWNLOAD_EXTRACT_TIMESTAMP TRUE  # Это решает проблему с предупреждением
#    )
#
#    # Для Windows: Предотвращаем переопределение макросов
#    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
#
#    # Делаем Google Test доступным
#    FetchContent_MakeAvailable(googletest)
#
#    # Создаем исполняемый файл для тестов
#    add_executable(sea_battle_tests
#            tests/test_main.cpp
#            tests/test_ships.cpp
#            tests/test_board.cpp
#            tests/test_fleet.cpp
#            tests/test_player.cpp
#            tests/test_game.cpp  # если создадите этот файл
#    )
#
#    # Добавляем директории с заголовками
#    target_include_directories(sea_battle_tests PRIVATE include)
#
#    # Линкуем с библиотеками Google Test
#    target_link_libraries(sea_battle_tests
#            GTest::gtest_main
#            GTest::gtest
#    )
#
#    # Включаем тестирование
#    include(GoogleTest)
#    gtest_discover_tests(sea_battle_tests)
#endif()