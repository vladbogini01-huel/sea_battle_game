#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <iostream>

const int BOARD_SIZE = 10;
//sings
const char EMPTY = '.';
const char SHIP = 'O';
const char HIT = 'X';
const char MISS = '*';
const char DESTROYED = '@';
const char UNKNOWN = '?';

// ships
const int BATTLESHIPS = 1;
const int CRUISERS = 2;
const int DESTROYERS = 3;
const int PATROL_BOATS = 4;

// sizes
const int BATTLESHIP_SIZE = 4;
const int CRUISER_SIZE = 3;
const int DESTROYER_SIZE = 2;
const int PATROL_BOAT_SIZE = 1;


const bool HORIZONTAL = true;
const bool VERTICAL = false;

// coordintaes


const char COL_LETTERS[BOARD_SIZE] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};


const int MIN_ROW = 1;
const int MAX_ROW = 10;
const char MIN_COL = 'A';
const char MAX_COL = 'J';

// letter into number
inline int colLetterToIndex(char letter) {

    return letter - 'A';
}

// number into letter
inline char colIndexToLetter(int index) {

    return 'A' + index;
}


// Очистка экрана
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Пауза до нажатия Enter
void waitForEnter() {
    std::cout << "\nНажмите Enter для продолжения...";
    std::cin.get();
}

#endif