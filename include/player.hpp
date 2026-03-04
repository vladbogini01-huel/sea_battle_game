#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <utility>
#include <iostream>

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
//просто стреляет по клеткам подряд
class DummyBotPlayer : public Player {
private:
    //Текущая позиция "куда следующий выстрел"
    int curRow = 0;
    int curCol = 0;

public:
    explicit DummyBotPlayer(const std::string& playerName)
        : Player(playerName) {}

    //Ставим корабли тестово
    void placeFleet() override {
        board.placeFleetTest();
    }

    //Следующая клетка по порядку: (0,0) потом (0,1) ... (0,9) потом (1,0) ...
    std::pair<int, int> selectShot() override {
        int r = curRow;
        int c = curCol;

        curCol++;
        if (curCol >= BOARD_SIZE) {
            curCol = 0;
            curRow++;
            if (curRow >= BOARD_SIZE) {
                curRow = 0;
            }
        }

        return {r, c};
    }
};

#endif