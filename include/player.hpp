#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <utility>
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

#include "board.hpp"
#include "constants.hpp"

// Player — базовый игрок.
class Player {
protected:
    //Имя игрока
    std::string name;

    //доска игрока
    Board board;

public:
   //создание игрока и доски
    explicit Player(const std::string& playerName)
        : name(playerName), board(playerName) {}

    virtual ~Player() = default;

    //геттер имени
    const std::string& getName() const { return name; }

    //Доступ к доске игрока
    Board& getBoard() { return board; }
    const Board& getBoard() const { return board; }

    //расставляем корабли
    virtual void placeFleet() = 0;

    //выбрать выстрел 
    virtual std::pair<int, int> selectShot() = 0;

    //отладка - показать поля игрока.
    virtual void printInfo() const {
        std::cout << "\n== " << name << " ==\n";
        board.printMyGrid();
        board.printEnemyGrid();
    }

protected:
    //переводим ввод вида ('A', 5) в индексы (row=4, col=0).
    //Возвращаем true если ввод нормальный, false если нет.
    bool parseShotInput(char colLetter, int rowNumber, int& rowIndex, int& colIndex) const {
        //Проверяем границы по константам
        if (colLetter < MIN_COL || colLetter > MAX_COL) return false;
        if (rowNumber < MIN_ROW || rowNumber > MAX_ROW) return false;

        colIndex = colLetterToIndex(colLetter);
        rowIndex = rowNumber - 1;
        return true;
    }
};

//игрок-человек.
class HumanPlayer : public Player {
public:
    explicit HumanPlayer(const std::string& playerName)
        : Player(playerName) {}

    //пока тестовая расстановка (как в Fleet)
    void placeFleet() override {
        board.placeFleetTest();
    }

    //запрос координаты
    //Пример: A 5
    std::pair<int, int> selectShot() override {
        while (true) {
            std::cout << name << ", enter shot (example: A 5): ";

            char col;
            int row;
            std::cin >> col >> row;

            int r = 0, c = 0;
            if (!parseShotInput(col, row, r, c)) {
                std::cout << "Bad input. Use A-J and 1-10.\n";
                continue;
            }

            return {r, c};
        }
    }
};

//бот
class DummyBotPlayer : public Player {
private:
    //текущая позиция "куда следующий выстрел"
    int curRow = 0;
    int curCol = 0;

    std::vector<std::pair<int, int>> shots;
    std::mt19937 gen{std::random_device{}()};
    
    void prepareShots() {
        shots.clear();

        for (int r = 0; r < BOARD_SIZE; r++) {
            for (int c = 0; c < BOARD_SIZE; c++) {
                shots.push_back({r, c});
            }
        }

        std::shuffle(shots.begin(), shots.end(), gen);

        curRow = 0;
        curCol = 0;
    }

public:
    explicit DummyBotPlayer(const std::string& playerName)
        : Player(playerName) {
        prepareShots();
    }
    //ставим корабли
    void placeFleet() override {
        board.placeFleetTest();
    }
    //следующая клетка выбирается не по порядку, а из перемешанного списка
    std::pair<int, int> selectShot() override {
        if (shots.empty() || curRow >= static_cast<int>(shots.size())) {
            prepareShots();
        }

        std::pair<int, int> shot = shots[curRow];
        curRow++;

        curCol = curRow;

        return shot;
    }
};

#endif