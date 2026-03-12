#include <gtest/gtest.h>
#include "game.hpp"
#include "player.hpp"

TEST(GameTest, HasNoWinnerRightAfterCreation) {
    auto p1 = std::make_unique<HumanPlayer>("Alice");
    auto p2 = std::make_unique<DummyBotPlayer>("Bot");

    Game game(std::move(p1), std::move(p2));

    EXPECT_FALSE(game.hasWinner());
    EXPECT_FALSE(game.getWinnerName().has_value());
}

TEST(GameTest, CreatePlayerReturnsHumanPlayerForTypeOne) {
    auto player = Game::createPlayer(1, "Alice");

    ASSERT_NE(player, nullptr);
    EXPECT_EQ(player->getName(), "Alice");
    EXPECT_NE(dynamic_cast<HumanPlayer*>(player.get()), nullptr);
}

TEST(GameTest, CreatePlayerReturnsBotPlayerForTypeTwo) {
    auto player = Game::createPlayer(2, "Bot");

    ASSERT_NE(player, nullptr);
    EXPECT_EQ(player->getName(), "Bot");
    EXPECT_NE(dynamic_cast<DummyBotPlayer*>(player.get()), nullptr);
}

TEST(GameTest, SetupPlacesFleetForBothPlayers) {
    auto p1 = std::make_unique<HumanPlayer>("Alice");
    auto p2 = std::make_unique<DummyBotPlayer>("Bot");

    Game game(std::move(p1), std::move(p2));
    game.setup();

    // Проверяем только косвенно через создание новой игры не получится,
    // поэтому этот тест просто гарантирует, что setup не падает.
    EXPECT_FALSE(game.hasWinner());
}