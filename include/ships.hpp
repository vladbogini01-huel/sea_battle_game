#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <utility>

class Ship {
protected:
    std::string name;
    int size;
    int hits;
    std::vector<std::pair<int, int>> cells;
    bool isHorizontal;
    bool isPlaced;

public:
    // constructor
    Ship(const std::string& shipName, int shipSize)
            : name(shipName),
              size(shipSize),
              hits(0),
              isHorizontal(true),
              isPlaced(false) {}

    // virtual destructor
    virtual ~Ship() = default;

    // Чисто виртуальные функции
    virtual std::string getType() const = 0;

    // Основной метод проверки размещения
    virtual bool canPlaceAt(int row, int col, bool horizontal,
                            const std::vector<std::vector<char>>& board) const {
        // Проверяем каждую клетку корабля
        for (int i = 0; i < size; i++) {
            int r = row + (horizontal ? 0 : i);
            int c = col + (horizontal ? i : 0);

            // Проверка границ
            if (!isWithinBounds(r, c))
                return false;

            // свободна ли клетка
            if (board[r][c] != '.')
                return false;

            // Проверка соседних клеток
            if (hasAdjacentShip(r, c, board))
                return false;
        }
        return true;
    }

    virtual void hit() {
        hits++;
    }

    virtual bool isDestroyed() const {
        return hits >= size;
    }

    virtual void placeAt(int row, int col, bool horizontal) {
        isHorizontal = horizontal;
        isPlaced = true;
        cells.clear();

        for (int i = 0; i < size; i++) {
            int r = row + (horizontal ? 0 : i);
            int c = col + (horizontal ? i : 0);
            cells.push_back({r, c});
        }
    }

    // getters
    std::string getName() const { return name; }
    int getSize() const { return size; }
    int getHits() const { return hits; }
    bool getPlaced() const { return isPlaced; }
    bool getOrientation() const { return isHorizontal; }
    const std::vector<std::pair<int, int>>& getCells() const { return cells; }

    // additional functions
    virtual void printInfo() const {
        std::cout << "Корабль: " << name << " (размер: " << size << ")\n";
        std::cout << "Состояние: " << (isDestroyed() ? "ПОТОПЛЕН" : "ЦЕЛ") << "\n";
        std::cout << "Попаданий: " << hits << "/" << size << "\n";
        if (isPlaced) {
            std::cout << "Клетки: ";
            for (auto& cell : cells) {
                std::cout << "(" << cell.first + 1 << ","
                          << static_cast<char>('A' + cell.second) << ") ";
            }
            std::cout << "\n";
        }
    }

    // reset method
    virtual void reset() {
        hits = 0;
        isPlaced = false;
        cells.clear();
    }

protected:

    // Проверка границ доски
    bool isWithinBounds(int row, int col) const {
        return (row >= 0 && row < 10 && col >= 0 && col < 10);
    }

    // Проверка соседних клеток
    bool hasAdjacentShip(int row, int col,
                         const std::vector<std::vector<char>>& board) const {
        for (int dr = -1; dr <= 1; dr++) {
            for (int dc = -1; dc <= 1; dc++) {
                if (dr == 0 && dc == 0) continue;

                int nr = row + dr;
                int nc = col + dc;

                if (isWithinBounds(nr, nc)) {
                    if (board[nr][nc] != '.' && board[nr][nc] != '?') {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    // можно ли разместить корабль в одной клетке
    bool canPlaceAtCell(int row, int col,
                        const std::vector<std::vector<char>>& board) const {
        return isWithinBounds(row, col) &&
               board[row][col] == '.' &&
               !hasAdjacentShip(row, col, board);
    }
};


// Однопалубный корабль
class PatrolBoat : public Ship {
public:
    PatrolBoat() : Ship("Катер", 1) {}

    std::string getType() const override {
        return "PatrolBoat";
    }
};

// Двухпалубный корабль
class Destroyer : public Ship {
public:
    Destroyer() : Ship("Эсминец", 2) {}

    std::string getType() const override {
        return "Destroyer";
    }
};

// Трехпалубный корабль
class Cruiser : public Ship {
public:
    Cruiser() : Ship("Крейсер", 3) {}

    std::string getType() const override {
        return "Cruiser";
    }
};

// Четырехпалубный корабль
class Battleship : public Ship {
public:
    Battleship() : Ship("Линкор", 4) {}

    std::string getType() const override {
        return "Battleship";
    }
};


