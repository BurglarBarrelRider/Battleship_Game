#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <map>
#include <algorithm>

using namespace std;

const int BOARD_SIZE = 7;
const int SHIP_COUNT[] = {1, 2, 4}; // Number of ships of lengths 3, 2, and 1
const int SHIP_LENGTHS[] = {3, 2, 1}; // Ship lengths

struct Ship {
    int length;
    vector<pair<int, int>> coordinates;
};

class BattleshipGame {
private:
    vector<vector<char>> board;
    map<string, int> players;
    
    string playerName; // var for Players name
    
    vector<Ship> ships;
    int shotsFired;

    void initializeBoard() {
        board.assign(BOARD_SIZE, vector<char>(BOARD_SIZE, ' '));
    }

   bool isValidPlacement(int row, int col, int length, bool isVertical) {
    int endRow = row + (isVertical ? length - 1 : 0);
    int endCol = col + (isVertical ? 0 : length - 1);

    if (endRow >= BOARD_SIZE || endCol >= BOARD_SIZE) {
        return false;
    }

    for (int i = max(0, row - 1); i <= min(BOARD_SIZE - 1, endRow + 1); ++i) {
        for (int j = max(0, col - 1); j <= min(BOARD_SIZE - 1, endCol + 1); ++j) {
            if (board[i][j] != ' ') {
                return false;
            }
        }
    }

    // Проверка углов соседних клеток
    // Угловые клетки для горизонтально расположенного корабля
    if (!isVertical) {
        if (row > 0 && col > 0 && board[row - 1][col - 1] != ' ') {
            return false; // Левый верхний угол
        }
        if (row > 0 && endCol < BOARD_SIZE - 1 && board[row - 1][endCol + 1] != ' ') {
            return false; // Правый верхний угол
        }
        if (endRow < BOARD_SIZE - 1 && col > 0 && board[endRow + 1][col - 1] != ' ') {
            return false; // Левый нижний угол
        }
        if (endRow < BOARD_SIZE - 1 && endCol < BOARD_SIZE - 1 && board[endRow + 1][endCol + 1] != ' ') {
            return false; // Правый нижний угол
        }
    }
    // Угловые клетки для вертикально расположенного корабля
    else {
        if (row > 0 && col > 0 && board[row - 1][col - 1] != ' ') {
            return false; // Левый верхний угол
        }
        if (endRow < BOARD_SIZE - 1 && col > 0 && board[endRow + 1][col - 1] != ' ') {
            return false; // Левый нижний угол
        }
        if (row > 0 && endCol < BOARD_SIZE - 1 && board[row - 1][endCol + 1] != ' ') {
            return false; // Правый верхний угол
        }
        if (endRow < BOARD_SIZE - 1 && endCol < BOARD_SIZE - 1 && board[endRow + 1][endCol + 1] != ' ') {
            return false; // Правый нижний угол
        }
    }

    return true;
}


    void placeShipsRandomly() {
        for (int length : SHIP_LENGTHS) {
            for (int i = 0; i < SHIP_COUNT[length - 1]; ++i) {
                Ship newShip;
                newShip.length = length;

                int row, col;
                bool isVertical;
                do {
                    row = rand() % BOARD_SIZE;
                    col = rand() % BOARD_SIZE;
                    isVertical = rand() % 2 == 0;
                } while (!isValidPlacement(row, col, length, isVertical));

                for (int k = 0; k < length; ++k) {
                    newShip.coordinates.push_back({row, col});
                    if (isVertical) {
                        row++;
                    } else {
                        col++;
                    }
                }

                ships.push_back(newShip);
            }
        }
    }

    bool isOutOfBounds(int row, int col) {
        return row < 0 || col < 0 || row >= BOARD_SIZE || col >= BOARD_SIZE;
    }

    bool isAlreadyGuessed(int row, int col) {
        return board[row][col] != ' ';
    }
    
    bool isShipHit(int row, int col) {
        for (auto& ship : ships) {
            for (auto& coord : ship.coordinates) {
                if (coord.first == row && coord.second == col) {
                    ship.coordinates.erase(remove(
                        ship.coordinates.begin(), ship.coordinates.end(), coord), ship.coordinates.end());
                    return true;
                }
            }
        }
        return false;
    }

    void markBoard(int row, int col, bool isHit, bool isSunk) {
    if (isSunk) {
            for (auto& ship : ships) {
                for (const auto& coord : ship.coordinates) {
                    board[coord.first][coord.second] = 'S'; // Пометка потопленного корабля на доске
                }
            }
        } else {
            if (isHit) {
                board[row][col] = 'X'; // Попадание
            } else {
                board[row][col] = 'O'; // Промах
            }
        }
    }

    void updateBoard() {
        system("clear"); // Очистка экрана (примечание: на Windows может быть нужна другая команда)

        cout << "   ";
        for (int i = 0; i < BOARD_SIZE; ++i) {
            cout << i << " ";
        }
        cout << endl;

        for (int i = 0; i < BOARD_SIZE; ++i) {
            cout << i << " |";
            for (int j = 0; j < BOARD_SIZE; ++j) {
                cout << board[i][j] << " ";
            }
            cout << endl;
        }
    }


    bool checkVictory() {
        for (const auto& ship : ships) {
            if (!ship.coordinates.empty()) {
                return false;
            }
        }
        return true;
    }

    void displayBoard() {
        cout << "   ";
        for (int i = 0; i < BOARD_SIZE; ++i) {
            cout << i << " ";
        }
        cout << endl;
        for (int i = 0; i < BOARD_SIZE; ++i) {
            cout << i << " |";
            for (int j = 0; j < BOARD_SIZE; ++j) {
                cout << board[i][j] << " ";
            }
            cout << endl;
        }
    }

public:
    BattleshipGame() {
        initializeBoard();
        cout << "Enter your name: ";
        cin >> playerName;
        placeShipsRandomly();
        shotsFired = 0;
    }

    void playGame() {
        while (true) {
            updateBoard();
            displayBoard();
            int row, col;
            cout << "Enter row and column to shoot (0-6): ";
            cin >> row >> col;

            if (isOutOfBounds(row, col)) {
                cout << "Invalid input! Please enter valid coordinates." << endl;
                continue;
            }

            if (isAlreadyGuessed(row, col)) {
                cout << "You've already guessed this coordinate! Try again." << endl;
                continue;
            }

            bool isHit = isShipHit(row, col);
            bool isSunk = markBoard(row, col, isHit);
            shotsFired++;

            if (isHit) {
                markBoard(row, col, true, isSunk);
                cout << "Hit!" << endl;
                if (checkVictory()) {
                    updateBoard();
                    cout << "You've sunk all the ships! Congratulations!" << endl;
                    cout << "Shots fired: " << shotsFired << endl;
                    break;
                }
            } else {
                markBoard(row, col, false, false);
                cout << "Miss!" << endl;
            }
        }
    }


int main() {
    srand(static_cast<unsigned int>(time(0)));

    BattleshipGame game;

    while (true) {
        game.playGame();

        string choice;
        cout << "Do you want to play again? (yes/no): ";
        cin >> choice;

        if (choice != "yes") {
            game.displayPlayerStatistics();
            break;
        }
    }

    return 0;
}
};
