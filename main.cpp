#include <iostream>
#include <memory>

#include "include/game.hpp"
#include "include/player.hpp"
#include "include/constants.hpp"

int main() {
    std::cout << "=== МОРСКОЙ БОЙ ===\n\n";

    int gameMode;
    std::cout << "Выберите режим игры:\n";
    std::cout << "1 - Игрок против игрока\n";
    std::cout << "2 - Игрок против компьютера\n";
    std::cout << "Ваш выбор: ";
    std::cin >> gameMode;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::unique_ptr<Player> player1, player2;
    std::string name1, name2;

    if (gameMode == 1) {
        std::cout << "Введите имя первого игрока: ";
        std::getline(std::cin, name1);
        std::cout << "Введите имя второго игрока: ";
        std::getline(std::cin, name2);

        player1 = std::make_unique<HumanPlayer>(name1);
        player2 = std::make_unique<HumanPlayer>(name2);
    } else {
        std::cout << "Введите ваше имя: ";
        std::getline(std::cin, name1);

        player1 = std::make_unique<HumanPlayer>(name1);
        player2 = std::make_unique<DummyBotPlayer>("Компьютер");
    }

    // Расстановка
    clearScreen();
    std::cout << "РАССТАНОВКА КОРАБЛЕЙ\n\n";

    if (gameMode == 1) {
        // PvP - показываем поля по очереди
        std::cout << name1 << " расставляет корабли:\n";
        player1->placeFleet();
        player1->getBoard().printMyGrid();  // Только своё поле
        waitForEnter();

        clearScreen();
        std::cout << name2 << " расставляет корабли:\n";
        player2->placeFleet();
        player2->getBoard().printMyGrid();  // Только своё поле
        waitForEnter();
    } else {
        // PvE - только игрок расставляет
        std::cout << "Расставьте свои корабли:\n";
        player1->placeFleet();
        player1->getBoard().printMyGrid();  // Только своё поле

        std::cout << "\nКомпьютер расставляет корабли...\n";
        player2->placeFleet();
        std::cout << "Готово!\n";
        waitForEnter();
    }

    // Создаём и запускаем игру
    Game game(std::move(player1), std::move(player2));

    clearScreen();
    std::cout << "ИГРА НАЧИНАЕТСЯ!\n\n";
    waitForEnter();

    game.run();

    return 0;
}