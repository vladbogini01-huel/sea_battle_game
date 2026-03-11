#include <gtest/gtest.h>
#include "../include/ships.hpp"
#include <vector>

// Тест создания кораблей
TEST(ShipTest, CreateShips) {
PatrolBoat patrol;
Destroyer destroyer;
Cruiser cruiser;
Battleship battleship;

EXPECT_EQ(patrol.getSize(), 1);
EXPECT_EQ(destroyer.getSize(), 2);
EXPECT_EQ(cruiser.getSize(), 3);
EXPECT_EQ(battleship.getSize(), 4);

EXPECT_EQ(patrol.getName(), "Катер");
EXPECT_EQ(destroyer.getName(), "Эсминец");
EXPECT_EQ(cruiser.getName(), "Крейсер");
EXPECT_EQ(battleship.getName(), "Линкор");
}

// Тест попаданий в корабль
TEST(ShipTest, Hits) {
Battleship ship;

EXPECT_FALSE(ship.isDestroyed());
EXPECT_EQ(ship.getHits(), 0);

ship.hit();
EXPECT_EQ(ship.getHits(), 1);
EXPECT_FALSE(ship.isDestroyed());

ship.hit();
ship.hit();
ship.hit();
EXPECT_EQ(ship.getHits(), 4);
EXPECT_TRUE(ship.isDestroyed());
}

// Тест размещения корабля
TEST(ShipTest, Placement) {
Cruiser ship;
std::vector<std::vector<char>> board(10, std::vector<char>(10, '.'));

// Горизонтальное размещение
EXPECT_TRUE(ship.canPlaceAt(2, 2, true, board));
ship.placeAt(2, 2, true);

auto cells = ship.getCells();
EXPECT_EQ(cells.size(), 3);
EXPECT_EQ(cells[0].first, 2);
EXPECT_EQ(cells[0].second, 2);
EXPECT_EQ(cells[1].second, 3);
EXPECT_EQ(cells[2].second, 4);

// Вертикальное размещение
ship.reset();
EXPECT_TRUE(ship.canPlaceAt(5, 5, false, board));
ship.placeAt(5, 5, false);

cells = ship.getCells();
EXPECT_EQ(cells[0].first, 5);
EXPECT_EQ(cells[1].first, 6);
EXPECT_EQ(cells[2].first, 7);
}

// Тест проверки границ
TEST(ShipTest, BoundsCheck) {
Battleship ship;
std::vector<std::vector<char>> board(10, std::vector<char>(10, '.'));

// За границами
EXPECT_FALSE(ship.canPlaceAt(8, 8, true, board));  // Выходит за правую границу
EXPECT_FALSE(ship.canPlaceAt(8, 8, false, board)); // Выходит за нижнюю границу
EXPECT_FALSE(ship.canPlaceAt(-1, 0, true, board));
}

// Тест соседних клеток
TEST(ShipTest, AdjacentCells) {
std::vector<std::vector<char>> board(10, std::vector<char>(10, '.'));
board[3][3] = 'O'; // Ставим корабль

Destroyer ship;

// Рядом с существующим кораблем
EXPECT_FALSE(ship.canPlaceAt(2, 2, true, board)); // Диагональ
EXPECT_FALSE(ship.canPlaceAt(3, 4, false, board)); // Рядом по горизонтали
EXPECT_FALSE(ship.canPlaceAt(4, 3, true, board)); // Рядом по вертикали

// Подальше от корабля
EXPECT_TRUE(ship.canPlaceAt(0, 0, true, board));
}