#ifndef GAME_HPP
#define GAME_HPP

#include <memory>
#include <optional>
#include <iostream>

#include "player.hpp"
#include "constants.hpp"

class Game {
private:
    std::unique_ptr<Player> p1;
    std::unique_ptr<Player> p2;

    //победитель 1 или 2, либо пусто, если ещё играем
    std::optional<int> winnerIndex;

public:
    //создаём игру с двумя игроками
    Game(std::unique_ptr<Player> a, std::unique_ptr<Player> b)
        : p1(std::move(a)), p2(std::move(b)), winnerIndex(std::nullopt) {}

    //оба игрока расставят корабли
    void setup() {
        p1->placeFleet();
        p2->placeFleet();
    }

    //быстрая проверка есть ли победитель
    bool hasWinner() const {
        return winnerIndex.has_value();
    }

    //если победитель есть - вернем его имя, иначе пусто
    std::optional<std::string> getWinnerName() const {
        if (!winnerIndex) return std::nullopt;

        if (*winnerIndex == 1) return p1->getName();
        return p2->getName();
    }

    //основной запуск игры:
    //1) setup
    //2) крутим ходы, пока кто-то не проиграет
    //3) печатаем победителя
    void run() {
        setup();

        int turn = 0;

        while (!winnerIndex) {
            turn++;
            clearScreen();

            if (turn % 2 == 1) {
                // Ход игрока 1
                std::cout << "\n🎯 ХОД " << p1->getName() << "\n";

                // Показываем оба поля рядом
                p1->getBoard().printBothGrids(p2->getBoard());

                bool hit = doTurn(*p1, *p2);

                // Если было попадание и игра не закончена, отменяем увеличение turn
                if (hit && !p2->getBoard().isFleetDead()) {
                    turn--;  // тот же игрок ходит снова
                    std::cout << "\n🎯 Попадание! Ещё один ход!\n";
                }

                // Статистика уже в printBothGrids, но добавим разделитель
                std::cout << "\n";

                if (p2->getBoard().isFleetDead()) {
                    winnerIndex = 1;
                }
            } else {
                // Ход игрока 2
                if (typeid(*p2) == typeid(HumanPlayer)) {
                    std::cout << "\n🎯 ХОД " << p2->getName() << "\n";
                    p2->getBoard().printBothGrids(p1->getBoard());
                } else {
                    // Компьютер
                    std::cout << "💻 ХОД КОМПЬЮТЕРА\n\n";
                    std::cout << "Ваше поле и ваши выстрелы:\n";
                    p1->getBoard().printBothGrids(p2->getBoard());
                }

                bool hit = doTurn(*p2, *p1);

                // Если было попадание и игра не закончена, отменяем увеличение turn
                if (hit && !p1->getBoard().isFleetDead()) {
                    turn--;  // тот же игрок ходит снова
                    if (typeid(*p2) == typeid(HumanPlayer)) {
                        std::cout << "\n🎯 Попадание! Ещё один ход!\n";
                    } else {
                        std::cout << "\n💻 Компьютер попал! Ходит снова...\n";
                    }
                }

                if (p1->getBoard().isFleetDead()) {
                    winnerIndex = 2;
                }
            }

            // Пауза только в PvP или если нужно
            if (typeid(*p1) == typeid(HumanPlayer) && typeid(*p2) == typeid(HumanPlayer)) {
                waitForEnter();  // принудительная пауза для PvP
            } else {
                waitForEnter();  // без паузы для PvE
            }
        }

        clearScreen();
        auto w = getWinnerName();
        if (w) {
            std::cout << " ПОБЕДИТЕЛЬ: " << *w << "\n";
            // Показываем финальные поля
            if (*winnerIndex == 1) {
                p1->getBoard().printBothGrids(p2->getBoard());
            } else {
                p2->getBoard().printBothGrids(p1->getBoard());
            }
        }
    }
    static std::unique_ptr<Player> createPlayer(int type, const std::string& name) {
        if (type == 2) {
            return std::make_unique<DummyBotPlayer>(name);
        }
        return std::make_unique<HumanPlayer>(name);
    }
private:
    //shooter выбирает координаты, target принимает выстрел
    bool doTurn(Player& shooter, Player& target) {
        auto [row, col] = shooter.selectShot();

        //если уже стреляли туда - !пропуск!
        if (shooter.getBoard().wasShotToEnemy(row, col)) {
            std::cout << shooter.getName() << " already shot there.\n";
            return false;  // возвращаем false, ход не засчитан
        }

        //target принимает выстрел
        //destroyed = true если корабль потопили этим попаданием
        bool destroyed = false;
        bool hit = target.getBoard().receiveShot(row, col, destroyed);

        //shooter запоминает у себя результат выстрела по врагу
        shooter.getBoard().markEnemyShot(row, col, hit, destroyed);

        //вывод происходящего
        std::cout << shooter.getName() << " shoots "
                  << colIndexToLetter(col) << " " << (row + 1)
                  << " -> " << (hit ? "HIT" : "MISS");

        if (hit && destroyed) std::cout << " (DESTROYED)";
        std::cout << "\n";

        return hit;  // возвращаем true при попадании
    }
};

#endif