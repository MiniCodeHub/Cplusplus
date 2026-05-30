#include <iostream>
#include <vector>

using namespace std;

class NQueens {

private:
    int n;
    vector<vector<char>> board;

public:
    NQueens(int size) {
        n = size;
        board.resize(
            n,
            vector<char>(n, '.')
        );
    }

    // Check if queen can be placed
    bool isSafe(
        int row,
        int col
    ) {

        // Upper Left Diagonal
        for (
            int i = row,
                j = col;
            i >= 0 && j >= 0;
            i--, j--
        ) {

            if (
                board[i][j] == 'Q'
            ) {
                return false;
            }
        }

        // Left Side
        for (
            int j = col;
            j >= 0;
            j--
        ) {

            if (
                board[row][j] == 'Q'
            ) {
                return false;
            }
        }

        // Lower Left Diagonal
        for (
            int i = row,
                j = col;
            i < n && j >= 0;
            i++, j--
        ) {

            if (
                board[i][j] == 'Q'
            ) {
                return false;
            }
        }

        return true;
    }

    // Backtracking Solver
    bool solve(int col) {

        if (col == n) {
            return true;
        }

        for (
            int row = 0;
            row < n;
            row++
        ) {

            if (
                isSafe(
                    row,
                    col
                )
            ) {

                board[row][col] = 'Q';

                if (
                    solve(col + 1)
                ) {
                    return true;
                }

                // Backtrack
                board[row][col] = '.';
            }
        }

        return false;
    }

    // Print Board
    void printBoard() {

        cout
            << "\nSolution:\n\n";

        for (
            int i = 0;
            i < n;
            i++
        ) {

            for (
                int j = 0;
                j < n;
                j++
            ) {

                cout
                    << board[i][j]
                    << " ";
            }

            cout << endl;
        }
    }
};

int main() {

    int n;

    cout
        << "Enter N: ";

    cin >> n;

    NQueens solver(n);

    if (
        solver.solve(0)
    ) {

        solver.printBoard();
    }
    else {

        cout
            << "\nNo solution exists.\n";
    }

    return 0;
}