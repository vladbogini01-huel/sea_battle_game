#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include <iostream>
#include <optional>

#include "constants.hpp"
#include "fleet.hpp"

class Board {
private:

    Fleet fleet;


    std::vector<std::vector<char>> myGrid;


    std::vector<std::vector<char>> enemyGrid;

    public:

    Board(const std::string& ownerName)
        : fleet(ownerName),
          myGrid(BOARD_SIZE, std::vector<char>(BOARD_SIZE, EMPTY)),
          enemyGrid(BOARD_SIZE, std::vector<char>(BOARD_SIZE, UNKNOWN)) {}

    Fleet& getFleet() { return fleet; }
    const Fleet& getFleet() const { return fleet; }

    //Проверка
    bool inBounds(int row, int col) const {
        return row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE;
    }

    //Расстановка кораблей 
    void placeFleetTest() {
        fleet.placeShipsTest();
        rebuildMyGridFromFleet();
    }

    //Рисуем myGrid по данным из Fleet.
    void rebuildMyGridFromFleet() {
        //очистка поля
        for (int r = 0; r < BOARD_SIZE; ++r) {
            for (int c = 0; c < BOARD_SIZE; ++c) {
                myGrid[r][c] = EMPTY;
            }
        }

        //проходим по кораблям и делаенм расстановку 
        for (int i = 0; i < fleet.getTotalCount(); ++i) {
            Ship* ship = fleet.getShip(i);
            if (!ship) continue;
            if (!ship->getPlaced()) continue;

            for (auto cell : ship->getCells()) {
                int row = cell.first;
                int col = cell.second;
                if (inBounds(row, col)) {
                    myGrid[row][col] = SHIP;
                }
            }
        }
    }
    //проверяем попадание и отмечаем на сетке
    bool receiveShot(int row, int col, bool& destroyed) {
        destroyed = false;

        if (!inBounds(row, col)) {
            return false;
        }

        bool hit = fleet.processShot(row, col, destroyed);

        if (hit) {
            myGrid[row][col] = HIT;

            //Если корабль потопили — пометим все клетки корабля как @
            if (destroyed) {
                Ship* s = fleet.findShipAt(row, col);
                if (s) {
                    for (auto cell : s->getCells()) {
                        int r = cell.first;
                        int c = cell.second;
                        if (inBounds(r, c)) {
                            myGrid[r][c] = DESTROYED;
                        }
                    }
                }
            }
        } else {
            //промах ставим *
            if (myGrid[row][col] == EMPTY) {
                myGrid[row][col] = MISS;
            }
        }

        return hit;
    }

    //запись собственных ходов
    void markEnemyShot(int row, int col, bool hit, bool destroyed) {
        if (!inBounds(row, col)) return;

        if (hit) {
            enemyGrid[row][col] = destroyed ? DESTROYED : HIT;
        } else {
            enemyGrid[row][col] = MISS;
        }
    }

    //проверка был ли выстрел в данную клетку
    bool wasShotToEnemy(int row, int col) const {
        if (!inBounds(row, col)) return false;
        return enemyGrid[row][col] != UNKNOWN;
    }

    //проверка на конец игры
    bool isFleetDead() const {
        return fleet.isAllDestroyed();
    }

    //Печать своего поля
    void printMyGrid(std::ostream& out = std::cout) const {
        out << "\nMY GRID:\n";
        printGrid(myGrid, out);
    }

    //печать своих выстрелов по врагу
    void printEnemyGrid(std::ostream& out = std::cout) const {
        out << "\nENEMY GRID (my shots):\n";
        printGrid(enemyGrid, out);
    }

// Печать двух полей рядом
    void printBothGrids(const Board& enemyBoard, std::ostream& out = std::cout) const {
        out << "\n" << std::string(50, '=') << "\n";

        // Заголовки
        out << "         ВАШЕ ПОЛЕ" << std::string(20, ' ') << "ВЫСТРЕЛЫ ПО ВРАГУ\n\n";

        // Буквы сверху для обоих полей
        out << "       ";
        for (int c = 0; c < BOARD_SIZE; ++c) {
            out << colIndexToLetter(c) << ' ';
        }
        out << "        ";  // разделитель
        for (int c = 0; c < BOARD_SIZE; ++c) {
            out << colIndexToLetter(c) << ' ';
        }
        out << "\n";

        // Верхняя рамка для обоих полей
        out << "      +";
        for (int i = 0; i < BOARD_SIZE * 2; ++i) out << '-';
        out << "+      +";
        for (int i = 0; i < BOARD_SIZE * 2; ++i) out << '-';
        out << "+\n";

        // Строки полей
        for (int r = 0; r < BOARD_SIZE; ++r) {
            // Номер слева
            out << "   ";
            if (r + 1 < 10) out << (r + 1) << ' ';
            else out << (r + 1);

            // Своё поле (myGrid)
            out << " |";
            for (int c = 0; c < BOARD_SIZE; ++c) {
                out << myGrid[r][c] << ' ';
            }
            out << "|   ";

            // Номер по центру
            if (r + 1 < 10) out << (r + 1) << ' ';
            else out << (r + 1);

            out << " |";
            for (int c = 0; c < BOARD_SIZE; ++c) {
                out << enemyGrid[r][c] << ' ';
            }
            out << "|";

            // Номер справа
            out << "   ";
            if (r + 1 < 10) out << (r + 1);
            else out << (r + 1);

            out << "\n";
        }

        // Нижняя рамка для обоих полей
        out << "      +";
        for (int i = 0; i < BOARD_SIZE * 2; ++i) out << '-';
        out << "+      +";
        for (int i = 0; i < BOARD_SIZE * 2; ++i) out << '-';
        out << "+\n";

        // Буквы снизу
        out << "       ";
        for (int c = 0; c < BOARD_SIZE; ++c) {
            out << colIndexToLetter(c) << ' ';
        }
        out << "        ";
        for (int c = 0; c < BOARD_SIZE; ++c) {
            out << colIndexToLetter(c) << ' ';
        }
        out << "\n\n";

        // Статистика
        out << "Кораблей: " << getFleet().getRemainingCount() << "/" << getFleet().getTotalCount()
            << "          Кораблей противника: " << enemyBoard.getFleet().getRemainingCount()
            << "/" << enemyBoard.getFleet().getTotalCount() << "\n";
        out << std::string(50, '=') << "\n";
    }

private:

    void printGrid(const std::vector<std::vector<char>>& g, std::ostream& out) const {
    //верхние буквы
    out << "       ";
    for (int c = 0; c < BOARD_SIZE; ++c) {
        out << colIndexToLetter(c) << ' ';
    }
    out << "\n";

    //верхняя рамка
    out << "   ";
    out << "  +";
    for (int i = 0; i < BOARD_SIZE * 2; ++i) out << '-';
    out << "+\n";

    //строки поля
    for (int r = 0; r < BOARD_SIZE; ++r) {
        //номер слева
        out << "   ";
        if (r + 1 < 10) out << (r + 1) << ' ';
        else out << (r + 1);

        out << " |";

        //клетки
        for (int c = 0; c < BOARD_SIZE; ++c) {
            out << g[r][c] << ' ';
        }

        out << "| ";

        //номер справа
        if (r + 1 < 10) out << (r + 1);
        else out << (r + 1);

        out << "\n";
    }

    //нижняя рамка
    out << "   ";
    out << "  +";
    for (int i = 0; i < BOARD_SIZE * 2; ++i) out << '-';
    out << "+\n";

    // Нижние буквы
    out << "       ";
    for (int c = 0; c < BOARD_SIZE; ++c) {
        out << colIndexToLetter(c) << ' ';
    }
    out << "\n";
}
};

#endif