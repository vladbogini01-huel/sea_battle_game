#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include <iostream>
#include <optional>

#include "constants.hpp"
#include "fleet.hpp"

// Board = поле игрока.
// Внутри 2 сетки (мое поле и мои выстрелы по врагу) + флот (корабли).
class Board {
private:
    // Флот игрока. В нем реально лежат корабли и их клетки.
    Fleet fleet;

    // Мое поле: тут видно где стоят свои корабли и куда по мне попали/промахнулись.
    std::vector<std::vector<char>> myGrid;

    // Поле “по врагу”: тут помечаем, куда стреляли и какой был результат.
    // Это просто заметки игрока, не реальное поле врага.
    std::vector<std::vector<char>> enemyGrid;
    
};

#endif