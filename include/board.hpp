#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include <iostream>
#include <optional>

#include "constants.hpp"
#include "fleet.hpp"

//Board = поле игрока.
//Внутри 2 сетки (поле и выстрелы по врагу) + флот
class Board {
private:
    //флот игрока. В нем лежат корабли и их клетки.
    Fleet fleet;

    //Наш поле со всеми заметками(выстрелы попадания и тп)
    std::vector<std::vector<char>> myGrid;

    //ОТмечаем куда стреляли и какой был результат.
    std::vector<std::vector<char>> enemyGrid;

    public:
    //Создаем доску для игрока:
    //флот делается с именем владельца
    //myGrid сначала пустое (.)
    //enemyGrid сначала неизвестно (?)
    Board(const std::string& ownerName)
        : fleet(ownerName),
          myGrid(BOARD_SIZE, std::vector<char>(BOARD_SIZE, EMPTY)),
          enemyGrid(BOARD_SIZE, std::vector<char>(BOARD_SIZE, UNKNOWN)) {}

    Fleet& getFleet() { return fleet; }
    const Fleet& getFleet() const { return fleet; }

    //Проверка, что координаты в пределах 0..9.
    bool inBounds(int row, int col) const {
        return row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE;
    }

    //Расстановка кораблей 
    void placeFleetTest() {
        fleet.placeShipsTest();
        rebuildMyGridFromFleet();
    }

    //Рисуем myGrid по данным из Fleet.
    //То есть Fleet хранит клетки кораблей, а myGrid — картинка.
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

private:
    //Одна общая функция печати сетки
    //Печатаем A..J сверху и 1..10 слева
    void printGrid(const std::vector<std::vector<char>>& g, std::ostream& out) const {
    //верхние буквы
    out << "     ";
    for (int c = 0; c < BOARD_SIZE; ++c) {
        out << colIndexToLetter(c) << ' ';
    }
    out << "\n\n";

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
    out << "+\n\n";

    // Нижние буквы
    out << "     ";
    for (int c = 0; c < BOARD_SIZE; ++c) {
        out << colIndexToLetter(c) << ' ';
    }
    out << "\n";
}
};

#endif