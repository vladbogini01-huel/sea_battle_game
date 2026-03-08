#ifndef FLEET_HPP
#define FLEET_HPP

#include "../include/ships.hpp"
#include <vector>
#include <memory>
#include <iostream>


class Fleet {
private:
    std::vector<std::unique_ptr<Ship>> ships;
    std::string ownerName;

public:

    Fleet(const std::string& owner) : ownerName(owner) {
        // 1 линкор
        ships.push_back(std::unique_ptr<Ship>(new Battleship()));
        // 2 крейсера
        ships.push_back(std::unique_ptr<Ship>(new Cruiser()));
        ships.push_back(std::unique_ptr<Ship>(new Cruiser()));
        // 3 эсминца
        ships.push_back(std::unique_ptr<Ship>(new Destroyer()));
        ships.push_back(std::unique_ptr<Ship>(new Destroyer()));
        ships.push_back(std::unique_ptr<Ship>(new Destroyer()));
        // 4 катера
        ships.push_back(std::unique_ptr<Ship>(new PatrolBoat()));
        ships.push_back(std::unique_ptr<Ship>(new PatrolBoat()));
        ships.push_back(std::unique_ptr<Ship>(new PatrolBoat()));
        ships.push_back(std::unique_ptr<Ship>(new PatrolBoat()));

        std::cout << "Создан флот для " << ownerName
                  << " из " << ships.size() << " кораблей\n";
    }

    // Сколько всего кораблей
    int getTotalCount() const {
        return ships.size();
    }

    // Сколько кораблей еще не потонули
    int getRemainingCount() const {
        int count = 0;
        for (const auto& ship : ships) {
            if (!(*ship).isDestroyed()) {
                count++;
            }
        }
        return count;
    }

    // все корабли потоплены?(
    bool isAllDestroyed() const {
        return getRemainingCount() == 0;
    }

    // Получить корабль по номеру
    Ship* getShip(int index) {
        if (index >= 0 && index < ships.size()) {
            return ships[index].get();
        }
        return nullptr;
    }

    // Найти корабль по координатам клетки
    Ship* findShipAt(int row, int col) {
        for (auto& ship : ships) {
            // Получаем сырой указатель
            Ship* rawShip = ship.get();

            // Проходим по всем клеткам корабля
            std::vector<std::pair<int, int>> cells = (*rawShip).getCells();

            for (auto& cell : cells) {
                if (cell.first == row && cell.second == col) {
                    return rawShip;  // нашли корабль в этой клетке
                }
            }
        }
        return nullptr;  // корабля нет
    }


    // Возвращает true если попадание, false если мимо
    // destroyed - будет true если корабль потоплен
    bool processShot(int row, int col, bool& destroyed) {
        Ship* ship = findShipAt(row, col);

        if (ship != nullptr) {
            // Попали в корабль
            (*ship).hit();
            destroyed = (*ship).isDestroyed();  // вместо ship->isDestroyed()

            if (destroyed) {
                std::cout << "Корабль " << (*ship).getName() << " потоплен!\n";  // вместо ship->getName()
            }

            return true;  // попадание
        }

        destroyed = false;
        return false;  // мимо
    }

    // Разместить все корабли на доске
    void placeShipsTest() {
            // Генератор случайных чисел
            unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
            std::mt19937 gen(seed);
            std::uniform_int_distribution<> posDist(0, 9);
            std::uniform_int_distribution<> dirDist(0, 1);

            // Создаем временную доску для проверки
            std::vector<std::vector<char>> tempBoard(10, std::vector<char>(10, EMPTY));

            // Для каждого корабля
            for (auto &ship: ships) {
                bool placed = false;
                int attempts = 0;
                const int maxAttempts = 1000;

                while (!placed && attempts < maxAttempts) {
                    int row = posDist(gen);
                    int col = posDist(gen);
                    bool horizontal = dirDist(gen) == 0;

                    // canplace ?
                    if ((*ship).canPlaceAt(row, col, horizontal, tempBoard)) {
                        (*ship).placeAt(row, col, horizontal);

                        // отмечаем на временной доске
                        for (auto cell: (*ship).getCells()) {
                            tempBoard[cell.first][cell.second] = SHIP;
                        }
                        placed = true;
                    }
                    attempts++;
                }
            }

            std::cout << "Флот для " << ownerName << " расставлен случайно\n";
        }
    // Показать состояние всех кораблей
    void printStatus() const {
        std::cout << "\n=== Флот игрока " << ownerName << " ===\n";
        std::cout << "Всего кораблей: " << ships.size() << "\n";
        std::cout << "Осталось: " << getRemainingCount() << "\n";

        int i = 1;
        for (const auto& ship : ships) {
            std::cout << i++ << ". ";
            (*ship).printInfo();
        }
    }

    // Получить имя владельца
    std::string getOwnerName() const {
        return ownerName;
    }

    // Сбросить все корабли
    void reset() {
        for (auto& ship : ships) {
            (*ship).reset();
        }
    }
};

#endif