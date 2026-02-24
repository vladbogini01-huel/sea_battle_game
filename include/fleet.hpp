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

    // Разместить все корабли на доске ( just test for now)
    void placeShipsTest() {
        // basic test
        if (ships.size() >= 1) (*ships[0]).placeAt(0, 0, true);    // линкор
        if (ships.size() >= 2) (*ships[1]).placeAt(2, 0, true);    // крейсер
        if (ships.size() >= 3) (*ships[2]).placeAt(4, 0, true);    // крейсер
        if (ships.size() >= 4) (*ships[3]).placeAt(6, 0, true);    // эсминец
        if (ships.size() >= 5) (*ships[4]).placeAt(7, 0, true);    // эсминец
        if (ships.size() >= 6) (*ships[5]).placeAt(8, 0, true);    // эсминец
        if (ships.size() >= 7) (*ships[6]).placeAt(9, 0, false);   // катер
        if (ships.size() >= 8) (*ships[7]).placeAt(9, 2, false);   // катер
        if (ships.size() >= 9) (*ships[8]).placeAt(9, 4, false);   // катер
        if (ships.size() >= 10) (*ships[9]).placeAt(9, 6, false);  // катер
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