#include <gtest/gtest.h>
#include "player.hpp"

#include <set>

class TestHumanPlayer : public HumanPlayer {
public:
    explicit TestHumanPlayer(const std::string& name)
        : HumanPlayer(name) {}

    bool testParseShotInput(char colLetter, int rowNumber, int& rowIndex, int& colIndex) {
        return parseShotInput(colLetter, rowNumber, rowIndex, colIndex);
    }
};

TEST(PlayerTest, HumanPlayerParseShotInputAcceptsValidInput) {
    TestHumanPlayer player("User");

    int row = -1;
    int col = -1;

    bool ok = player.testParseShotInput('C', 5, row, col);

    EXPECT_TRUE(ok);
    EXPECT_EQ(row, 4);
    EXPECT_EQ(col, 2);
}

TEST(PlayerTest, HumanPlayerParseShotInputRejectsInvalidColumn) {
    TestHumanPlayer player("User");

    int row = -1;
    int col = -1;

    EXPECT_FALSE(player.testParseShotInput('Z', 5, row, col));
    EXPECT_FALSE(player.testParseShotInput('@', 5, row, col));
}

TEST(PlayerTest, HumanPlayerParseShotInputRejectsInvalidRow) {
    TestHumanPlayer player("User");

    int row = -1;
    int col = -1;

    EXPECT_FALSE(player.testParseShotInput('A', 0, row, col));
    EXPECT_FALSE(player.testParseShotInput('A', 11, row, col));
}

TEST(PlayerTest, DummyBotPlayerReturnsValidCoordinates) {
    DummyBotPlayer bot("Bot");

    for (int i = 0; i < 100; ++i) {
        auto [row, col] = bot.selectShot();
        EXPECT_GE(row, 0);
        EXPECT_LT(row, BOARD_SIZE);
        EXPECT_GE(col, 0);
        EXPECT_LT(col, BOARD_SIZE);
    }
}

TEST(PlayerTest, DummyBotPlayerFirstHundredShotsAreUnique) {
    DummyBotPlayer bot("Bot");

    std::set<std::pair<int, int>> uniqueShots;

    for (int i = 0; i < 100; ++i) {
        uniqueShots.insert(bot.selectShot());
    }

    EXPECT_EQ(uniqueShots.size(), 100u);
}

TEST(PlayerTest, PlaceFleetMakesShipsPlacedForBot) {
    DummyBotPlayer bot("Bot");

    bot.placeFleet();

    int placedCount = 0;
    for (int i = 0; i < bot.getBoard().getFleet().getTotalCount(); ++i) {
        Ship* ship = bot.getBoard().getFleet().getShip(i);
        ASSERT_NE(ship, nullptr);
        if (ship->getPlaced()) {
            placedCount++;
        }
    }

    EXPECT_EQ(placedCount, 10);
}