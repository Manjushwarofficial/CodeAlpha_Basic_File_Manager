#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <limits>
using namespace std;

class SudokuSolver {
private:
    std::vector<std::vector<int>> board;
    const int EMPTY = 0;
    const int SIZE = 9;

    bool isValid(int row, int col, int num) {
        for (int x = 0; x < SIZE; x++) {
            if (board[row][x] == num) return false;
            if (board[x][col] == num) return false;
            if (board[3 * (row / 3) + x / 3][3 * (col / 3) + x % 3] == num) return false;
        }
        return true;
    }

    bool solve() {
        for (int row = 0; row < SIZE; row++) {
            for (int col = 0; col < SIZE; col++) {
                if (board[row][col] == EMPTY) {
                    for (int num = 1; num <= 9; num++) {
                        if (isValid(row, col, num)) {
                            board[row][col] = num;
                            if (solve()) return true;
                            board[row][col] = EMPTY;
                        }
                    }
                    return false;
                }
            }
        }
        return true;
    }

public:
    SudokuSolver(const std::vector<std::vector<int>>& initial_board) : board(initial_board) {}

    bool solveSudoku() {
        return solve();
    }

    void printBoard() {
        for (int i = 0; i < SIZE; i++) {
            if (i % 3 == 0 && i != 0) std::cout << "---------------------\n";
            for (int j = 0; j < SIZE; j++) {
                if (j % 3 == 0 && j != 0) std::cout << "| ";
                if (board[i][j] == EMPTY) std::cout << ". ";
                else std::cout << board[i][j] << " ";
            }
            std::cout << "\n";
        }
    }
};

std::vector<std::vector<int>> inputBoard() {
    std::vector<std::vector<int>> board(9, std::vector<int>(9));
    std::cout << "Enter the Sudoku puzzle, use 0 or . for empty cells.\n";
    std::cout << "Enter each row on a new line, separating numbers with spaces:\n";

    for (int i = 0; i < 9; i++) {
        std::string line;
        std::getline(std::cin, line);
        std::istringstream iss(line);
        for (int j = 0; j < 9; j++) {
            std::string cell;
            iss >> cell;
            if (cell == "." || cell == "0") board[i][j] = 0;
            else board[i][j] = std::stoi(cell);
        }
    }
    return board;
}

class SudokuInterface {
private:
    SudokuSolver* solver;

    void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }

    void waitForEnter() {
        std::cout << "\nPress Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::vector<std::vector<int>> getPresetPuzzle() {
        return {
            {5, 3, 0, 0, 7, 0, 0, 0, 0},
            {6, 0, 0, 1, 9, 5, 0, 0, 0},
            {0, 9, 8, 0, 0, 0, 0, 6, 0},
            {8, 0, 0, 0, 6, 0, 0, 0, 3},
            {4, 0, 0, 8, 0, 3, 0, 0, 1},
            {7, 0, 0, 0, 2, 0, 0, 0, 6},
            {0, 6, 0, 0, 0, 0, 2, 8, 0},
            {0, 0, 0, 4, 1, 9, 0, 0, 5},
            {0, 0, 0, 0, 8, 0, 0, 7, 9}
        };
    }

public:
    SudokuInterface() : solver(nullptr) {}

    ~SudokuInterface() {
        if (solver) delete solver;
    }

    void run() {
        while (true) {
            clearScreen();
            std::cout << "Welcome to Sudoku Solver!\n\n";
            std::cout << "1. Input your own puzzle\n";
            std::cout << "2. Use a preset puzzle\n";
            std::cout << "3. Exit\n\n";
            std::cout << "Enter your choice: ";

            int choice;
            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::vector<std::vector<int>> board;

            switch (choice) {
                case 1:
                    board = inputBoard();
                    break;
                case 2:
                    board = getPresetPuzzle();
                    break;
                case 3:
                    std::cout << "Thank you for using Sudoku Solver. Goodbye!\n";
                    return;
                default:
                    std::cout << "Invalid choice. Please try again.\n";
                    waitForEnter();
                    continue;
            }

            if (solver) delete solver;
            solver = new SudokuSolver(board);

            clearScreen();
            std::cout << "Original Puzzle:\n";
            solver->printBoard();

            if (solver->solveSudoku()) {
                std::cout << "\nSolution:\n";
                solver->printBoard();
            } else {
                std::cout << "\nNo solution exists for this puzzle.\n";
            }

            waitForEnter();
        }
    }
};

int main() {
    SudokuInterface interface;
    interface.run();
    return 0;
}