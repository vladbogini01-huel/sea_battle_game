#include <gtest/gtest.h>
#include "fleet.hpp"
#include "constants.hpp"

TEST(FleetTest, FleetHasTenShipsAfterCreation) {
    Fleet fleet("Player");

    EXPECT_EQ(fleet.getTotalCount(), 10);
    EXPECT_EQ(fleet.getRemainingCount(), 10);
    EXPECT_FALSE(fleet.isAllDestroyed());
}

TEST(FleetTest, GetShipReturnsNullForInvalidIndex) {
    Fleet fleet("Player");

    EXPECT_EQ(fleet.getShip(-1), nullptr);
    EXPECT_EQ(fleet.getShip(10), nullptr);
}

TEST(FleetTest, GetShipReturnsCorrectTypesByStandardOrder) {
    Fleet fleet("Player");

    ASSERT_NE(fleet.getShip(0), nullptr);
    ASSERT_NE(fleet.getShip(1), nullptr);
    ASSERT_NE(fleet.getShip(3), nullptr);
    ASSERT_NE(fleet.getShip(6), nullptr);

    EXPECT_EQ(fleet.getShip(0)->getType(), "Battleship");
    EXPECT_EQ(fleet.getShip(1)->getType(), "Cruiser");
    EXPECT_EQ(fleet.getShip(3)->getType(), "Destroyer");
    EXPECT_EQ(fleet.getShip(6)->getType(), "PatrolBoat");
}

TEST(FleetTest, FindShipAtReturnsShipWhenCellBelongsToShip) {
    Fleet fleet("Player");

    Ship* ship = fleet.getShip(0);
    ASSERT_NE(ship, nullptr);

    ship->placeAt(2, 2, true);

    Ship* found = fleet.findShipAt(2, 2);

    ASSERT_NE(found, nullptr);
    EXPECT_EQ(found, ship);
}

TEST(FleetTest, FindShipAtReturnsNullWhenNoShipAtCell) {
    Fleet fleet("Player");

    fleet.getShip(0)->placeAt(0, 0, true);

    EXPECT_EQ(fleet.findShipAt(9, 9), nullptr);
}

TEST(FleetTest, ProcessShotReturnsFalseOnEmptyCell) {
    Fleet fleet("Player");

    fleet.getShip(0)->placeAt(0, 0, true);

    bool destroyed = true;
    bool hit = fleet.processShot(9, 9, destroyed);

    EXPECT_FALSE(hit);
    EXPECT_FALSE(destroyed);
}

TEST(FleetTest, ProcessShotReturnsTrueOnShipCell) {
    Fleet fleet("Player");

    Ship* ship = fleet.getShip(6);
    ASSERT_NE(ship, nullptr);
    ship->placeAt(4, 4, true);

    bool destroyed = false;
    bool hit = fleet.processShot(4, 4, destroyed);

    EXPECT_TRUE(hit);
    EXPECT_TRUE(destroyed);
}

TEST(FleetTest, RemainingCountDecreasesWhenShipDestroyed) {
    Fleet fleet("Player");

    Ship* patrol = fleet.getShip(6);
    ASSERT_NE(patrol, nullptr);

    patrol->placeAt(1, 1, true);

    EXPECT_EQ(fleet.getRemainingCount(), 10);

    bool destroyed = false;
    ASSERT_TRUE(fleet.processShot(1, 1, destroyed));

    EXPECT_TRUE(destroyed);
    EXPECT_EQ(fleet.getRemainingCount(), 9);
}

TEST(FleetTest, IsAllDestroyedReturnsTrueWhenAllShipsDestroyed) {
    Fleet fleet("Player");

    for (int i = 0; i < fleet.getTotalCount(); ++i) {
        Ship* ship = fleet.getShip(i);
        ASSERT_NE(ship, nullptr);

        int row = i;
        int col = 0;
        ship->placeAt(row, col, true);

        for (const auto& cell : ship->getCells()) {
            bool destroyed = false;
            EXPECT_TRUE(fleet.processShot(cell.first, cell.second, destroyed));
        }
    }

    EXPECT_TRUE(fleet.isAllDestroyed());
    EXPECT_EQ(fleet.getRemainingCount(), 0);
}

TEST(FleetTest, ResetRestoresAllShipsToAliveAndUnplaced) {
    Fleet fleet("Player");

    Ship* ship = fleet.getShip(0);
    ASSERT_NE(ship, nullptr);

    ship->placeAt(0, 0, true);

    bool destroyed = false;
    fleet.processShot(0, 0, destroyed);
    fleet.processShot(0, 1, destroyed);
    fleet.processShot(0, 2, destroyed);
    fleet.processShot(0, 3, destroyed);

    fleet.reset();

    EXPECT_EQ(fleet.getRemainingCount(), 10);
    EXPECT_FALSE(fleet.isAllDestroyed());

    for (int i = 0; i < fleet.getTotalCount(); ++i) {
        Ship* current = fleet.getShip(i);
        ASSERT_NE(current, nullptr);
        EXPECT_EQ(current->getHits(), 0);
        EXPECT_FALSE(current->getPlaced());
        EXPECT_TRUE(current->getCells().empty());
    }
}