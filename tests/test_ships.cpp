#include <gtest/gtest.h>
#include "ships.hpp"
#include "constants.hpp"

#include <vector>

TEST(ShipTest, BattleshipHasCorrectSize) {
    Battleship ship;
    EXPECT_EQ(ship.getSize(), 4);
    EXPECT_EQ(ship.getType(), "Battleship");
    EXPECT_FALSE(ship.isDestroyed());
    EXPECT_FALSE(ship.getPlaced());
}

TEST(ShipTest, PatrolBoatDestroyedAfterOneHit) {
    PatrolBoat ship;

    EXPECT_FALSE(ship.isDestroyed());

    ship.hit();

    EXPECT_TRUE(ship.isDestroyed());
    EXPECT_EQ(ship.getHits(), 1);
}

TEST(ShipTest, DestroyerDestroyedAfterTwoHits) {
    Destroyer ship;

    ship.hit();
    EXPECT_FALSE(ship.isDestroyed());

    ship.hit();
    EXPECT_TRUE(ship.isDestroyed());
}

TEST(ShipTest, PlaceAtHorizontalSetsCorrectCells) {
    Cruiser ship;
    ship.placeAt(2, 3, true);

    ASSERT_TRUE(ship.getPlaced());
    ASSERT_TRUE(ship.getOrientation());

    const auto& cells = ship.getCells();
    ASSERT_EQ(cells.size(), 3u);

    EXPECT_EQ(cells[0], std::make_pair(2, 3));
    EXPECT_EQ(cells[1], std::make_pair(2, 4));
    EXPECT_EQ(cells[2], std::make_pair(2, 5));
}

TEST(ShipTest, PlaceAtVerticalSetsCorrectCells) {
    Destroyer ship;
    ship.placeAt(4, 1, false);

    ASSERT_TRUE(ship.getPlaced());
    EXPECT_FALSE(ship.getOrientation());

    const auto& cells = ship.getCells();
    ASSERT_EQ(cells.size(), 2u);

    EXPECT_EQ(cells[0], std::make_pair(4, 1));
    EXPECT_EQ(cells[1], std::make_pair(5, 1));
}

TEST(ShipTest, CanPlaceAtReturnsFalseWhenOutOfBoundsHorizontal) {
    Battleship ship;
    std::vector<std::vector<char>> board(BOARD_SIZE, std::vector<char>(BOARD_SIZE, EMPTY));

    EXPECT_FALSE(ship.canPlaceAt(0, 8, true, board));
}

TEST(ShipTest, CanPlaceAtReturnsFalseWhenOutOfBoundsVertical) {
    Battleship ship;
    std::vector<std::vector<char>> board(BOARD_SIZE, std::vector<char>(BOARD_SIZE, EMPTY));

    EXPECT_FALSE(ship.canPlaceAt(8, 0, false, board));
}

TEST(ShipTest, CanPlaceAtReturnsFalseWhenCellOccupied) {
    Cruiser ship;
    std::vector<std::vector<char>> board(BOARD_SIZE, std::vector<char>(BOARD_SIZE, EMPTY));
    board[3][3] = SHIP;

    EXPECT_FALSE(ship.canPlaceAt(3, 2, true, board));
}

TEST(ShipTest, CanPlaceAtReturnsFalseWhenAdjacentToAnotherShip) {
    PatrolBoat ship;
    std::vector<std::vector<char>> board(BOARD_SIZE, std::vector<char>(BOARD_SIZE, EMPTY));
    board[5][5] = SHIP;

    EXPECT_FALSE(ship.canPlaceAt(4, 4, true, board));
    EXPECT_FALSE(ship.canPlaceAt(4, 5, true, board));
    EXPECT_FALSE(ship.canPlaceAt(4, 6, true, board));
    EXPECT_FALSE(ship.canPlaceAt(5, 4, true, board));
    EXPECT_FALSE(ship.canPlaceAt(5, 6, true, board));
    EXPECT_FALSE(ship.canPlaceAt(6, 4, true, board));
    EXPECT_FALSE(ship.canPlaceAt(6, 5, true, board));
    EXPECT_FALSE(ship.canPlaceAt(6, 6, true, board));
}

TEST(ShipTest, ResetClearsHitsAndPlacement) {
    Cruiser ship;
    ship.placeAt(1, 1, true);
    ship.hit();
    ship.hit();

    ship.reset();

    EXPECT_EQ(ship.getHits(), 0);
    EXPECT_FALSE(ship.getPlaced());
    EXPECT_TRUE(ship.getCells().empty());
    EXPECT_FALSE(ship.isDestroyed());
}