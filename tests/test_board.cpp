#include <gtest/gtest.h>
#include "board.hpp"
#include "constants.hpp"

#include <sstream>

TEST(BoardTest, InBoundsReturnsTrueForValidCells) {
    Board board("Player");

    EXPECT_TRUE(board.inBounds(0, 0));
    EXPECT_TRUE(board.inBounds(9, 9));
    EXPECT_TRUE(board.inBounds(4, 7));
}

TEST(BoardTest, InBoundsReturnsFalseForInvalidCells) {
    Board board("Player");

    EXPECT_FALSE(board.inBounds(-1, 0));
    EXPECT_FALSE(board.inBounds(0, -1));
    EXPECT_FALSE(board.inBounds(10, 0));
    EXPECT_FALSE(board.inBounds(0, 10));
}

TEST(BoardTest, ReceiveShotReturnsFalseForOutOfBounds) {
    Board board("Player");

    bool destroyed = true;
    bool hit = board.receiveShot(-1, 0, destroyed);

    EXPECT_FALSE(hit);
    EXPECT_FALSE(destroyed);
}

TEST(BoardTest, ReceiveShotReturnsFalseOnEmptyCellAndMarksMiss) {
    Board board("Player");

    bool destroyed = true;
    bool hit = board.receiveShot(5, 5, destroyed);

    EXPECT_FALSE(hit);
    EXPECT_FALSE(destroyed);

    std::ostringstream out;
    board.printMyGrid(out);

    EXPECT_NE(out.str().find('*'), std::string::npos);
}

TEST(BoardTest, ReceiveShotReturnsTrueOnShipCell) {
    Board board("Player");

    Ship* patrol = board.getFleet().getShip(6);
    ASSERT_NE(patrol, nullptr);
    patrol->placeAt(2, 2, true);
    board.rebuildMyGridFromFleet();

    bool destroyed = false;
    bool hit = board.receiveShot(2, 2, destroyed);

    EXPECT_TRUE(hit);
    EXPECT_TRUE(destroyed);
}

TEST(BoardTest, ReceiveShotMarksDestroyedShipCells) {
    Board board("Player");

    Ship* destroyer = board.getFleet().getShip(3);
    ASSERT_NE(destroyer, nullptr);
    destroyer->placeAt(1, 1, true);
    board.rebuildMyGridFromFleet();

    bool destroyed = false;

    EXPECT_TRUE(board.receiveShot(1, 1, destroyed));
    EXPECT_FALSE(destroyed);

    EXPECT_TRUE(board.receiveShot(1, 2, destroyed));
    EXPECT_TRUE(destroyed);

    std::ostringstream out;
    board.printMyGrid(out);
    std::string text = out.str();

    EXPECT_NE(text.find('@'), std::string::npos);
}

TEST(BoardTest, MarkEnemyShotMarksMiss) {
    Board board("Player");

    board.markEnemyShot(3, 3, false, false);

    EXPECT_TRUE(board.wasShotToEnemy(3, 3));

    std::ostringstream out;
    board.printEnemyGrid(out);
    EXPECT_NE(out.str().find('*'), std::string::npos);
}

TEST(BoardTest, MarkEnemyShotMarksHit) {
    Board board("Player");

    board.markEnemyShot(4, 4, true, false);

    EXPECT_TRUE(board.wasShotToEnemy(4, 4));

    std::ostringstream out;
    board.printEnemyGrid(out);
    EXPECT_NE(out.str().find('X'), std::string::npos);
}

TEST(BoardTest, MarkEnemyShotMarksDestroyed) {
    Board board("Player");

    board.markEnemyShot(6, 6, true, true);

    EXPECT_TRUE(board.wasShotToEnemy(6, 6));

    std::ostringstream out;
    board.printEnemyGrid(out);
    EXPECT_NE(out.str().find('@'), std::string::npos);
}

TEST(BoardTest, WasShotToEnemyReturnsFalseForUnknownCell) {
    Board board("Player");

    EXPECT_FALSE(board.wasShotToEnemy(0, 0));
    EXPECT_FALSE(board.wasShotToEnemy(9, 9));
}

TEST(BoardTest, IsFleetDeadReturnsFalseAtStart) {
    Board board("Player");

    EXPECT_FALSE(board.isFleetDead());
}

TEST(BoardTest, IsFleetDeadReturnsTrueWhenAllShipsDestroyed) {
    Board board("Player");

    for (int i = 0; i < board.getFleet().getTotalCount(); ++i) {
        Ship* ship = board.getFleet().getShip(i);
        ASSERT_NE(ship, nullptr);

        ship->placeAt(i, 0, true);
    }
    board.rebuildMyGridFromFleet();

    for (int i = 0; i < board.getFleet().getTotalCount(); ++i) {
        Ship* ship = board.getFleet().getShip(i);
        ASSERT_NE(ship, nullptr);

        for (const auto& cell : ship->getCells()) {
            bool destroyed = false;
            EXPECT_TRUE(board.receiveShot(cell.first, cell.second, destroyed));
        }
    }

    EXPECT_TRUE(board.isFleetDead());
}

TEST(BoardTest, PlaceFleetTestActuallyPlacesShips) {
    Board board("Player");

    board.placeFleetTest();

    bool foundPlaced = false;
    for (int i = 0; i < board.getFleet().getTotalCount(); ++i) {
        Ship* ship = board.getFleet().getShip(i);
        ASSERT_NE(ship, nullptr);
        if (ship->getPlaced()) {
            foundPlaced = true;
        }
    }

    EXPECT_TRUE(foundPlaced);
}