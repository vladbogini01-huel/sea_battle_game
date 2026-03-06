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

            //нечётный ход - игрок 1, чётный - игрок 2
            if (turn % 2 == 1) {
                doTurn(*p1, *p2);

                //если флот второго уничтожен - победил первый
                if (p2->getBoard().isFleetDead()) {
                    winnerIndex = 1;
                }
            } else {
                doTurn(*p2, *p1);

                //если флот первого уничтожен - победил второй
                if (p1->getBoard().isFleetDead()) {
                    winnerIndex = 2;
                }
            }
        }

        auto w = getWinnerName();
        if (w) {
            std::cout << "\nWINNER: " << *w << "\n";
        }
    }

private:
    //shooter выбирает координаты, target принимает выстрел
    //shooter потом отмечает результат у себя на enemyGrid
    void doTurn(Player& shooter, Player& target) {
        auto [row, col] = shooter.selectShot();

        //если уже стреляли туда - !пропуск!
        if (shooter.getBoard().wasShotToEnemy(row, col)) {
            std::cout << shooter.getName() << " already shot there.\n";
            return;
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
    }
};

#endif