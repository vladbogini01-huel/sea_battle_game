cmake_minimum_required(VERSION 4.0)
project(sea_battle_game)

set(CMAKE_CXX_STANDARD 20)

add_executable(sea_battle_game main.cpp
        include/fleet.hpp
        include/ships.hpp
        include/constants.hpp
        include/board.hpp
        include/player.hpp
        include/game.hpp
)
