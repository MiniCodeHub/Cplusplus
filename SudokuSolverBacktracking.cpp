#include <iostream>

using namespace std;

const int SIZE = 9;

class SudokuSolver {

public:

    // Print Sudoku Board
    void printBoard(
        int board[SIZE][SIZE]
    ) {

        cout << "\nSolved Sudoku:\n\n";

        for (int row = 0; row < SIZE; row++) {

            for (int col = 0; col < SIZE; col++) {

                cout
                    << board[row][col]
                    << " ";
            }

            cout << endl;
        }
    }

    // Check Row
    bool isRowValid(
        int board[SIZE][SIZE],
        int row,
        int number
    ) {

        for (int col = 0; col < SIZE; col++) {

            if (
                board[row][col]
                == number
            ) {

                return false;
            }
        }

        return true;
    }

    // Check Column
    bool isColumnValid(
        int board[SIZE][SIZE],
        int col,
        int number
    ) {

        for (int row = 0; row < SIZE; row++) {

            if (
                board[row][col]
                == number
            ) {

                return false;
            }
        }

        return true;
    }

    // Check 3x3 Box
    bool isBoxValid(
        int board[SIZE][SIZE],
        int startRow,
        int startCol,
        int number
    ) {

        for (
            int row = 0;
            row < 3;
            row++
        ) {

            for (
                int col = 0;
                col < 3;
                col++
            ) {

                if (
                    board[
                        row + startRow
                    ][
                        col + startCol
                    ] == number
                ) {

                    return false;
                }
            }
        }

        return true;
    }

    // Check Valid Placement
    bool isSafe(
        int board[SIZE][SIZE],
        int row,
        int col,
        int number
    ) {

        return
            isRowValid(
                board,
                row,
                number
            ) &&

            isColumnValid(
                board,
                col,
                number
            ) &&

            isBoxValid(
                board,
                row - row % 3,
                col - col % 3,
                number
            );
    }

    // Solve Sudoku
    bool solve(
        int board[SIZE][SIZE]
    ) {

        int row;
        int col;

        bool emptyFound = false;

        // Find Empty Cell
        for (
            row = 0;
            row < SIZE;
            row++
        ) {

            for (
                col = 0;
                col < SIZE;
                col++
            ) {

                if (
                    board[row][col]
                    == 0
                ) {

                    emptyFound = true;
                    break;
                }
            }

            if (emptyFound) {
                break;
            }
        }

        // Puzzle Solved
        if (!emptyFound) {

            return true;
        }

        // Try Numbers 1-9
        for (
            int number = 1;
            number <= 9;
            number++
        ) {

            if (
                isSafe(
                    board,
                    row,
                    col,
                    number
                )
            ) {

                board[row][col] = number;

                // Recursive Solve
                if (
                    solve(board)
                ) {

                    return true;
                }

                // Backtrack
                board[row][col] = 0;
            }
        }

        return false;
    }
};

int main() {

    int board[SIZE][SIZE] = {

        {5,3,0,0,7,0,0,0,0},
        {6,0,0,1,9,5,0,0,0},
        {0,9,8,0,0,0,0,6,0},

        {8,0,0,0,6,0,0,0,3},
        {4,0,0,8,0,3,0,0,1},
        {7,0,0,0,2,0,0,0,6},

        {0,6,0,0,0,0,2,8,0},
        {0,0,0,4,1,9,0,0,5},
        {0,0,0,0,8,0,0,7,9}
    };

    SudokuSolver solver;

    cout << "Original Sudoku:\n\n";

    for (int row = 0; row < SIZE; row++) {

        for (int col = 0; col < SIZE; col++) {

            cout
                << board[row][col]
                << " ";
        }

        cout << endl;
    }

    bool solved =
        solver.solve(board);

    if (solved) {

        solver.printBoard(board);
    }
    else {

        cout
            << "\nNo Solution Exists\n";
    }

    return 0;
}