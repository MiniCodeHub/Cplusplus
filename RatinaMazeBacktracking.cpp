#include <iostream>
#include <vector>

using namespace std;

const int N = 4;

bool isSafe(
    int x,
    int y,
    vector<vector<int>>& maze
) {
    return (
        x >= 0 &&
        x < N &&
        y >= 0 &&
        y < N &&
        maze[x][y] == 1
    );
}

bool solveMaze(
    int x,
    int y,
    vector<vector<int>>& maze,
    vector<vector<int>>& solution
) {
    if (
        x == N - 1 &&
        y == N - 1 &&
        maze[x][y] == 1
    ) {
        solution[x][y] = 1;
        return true;
    }

    if (isSafe(x, y, maze)) {

        if (solution[x][y] == 1)
            return false;

        solution[x][y] = 1;

        // Down
        if (
            solveMaze(
                x + 1,
                y,
                maze,
                solution
            )
        )
            return true;

        // Right
        if (
            solveMaze(
                x,
                y + 1,
                maze,
                solution
            )
        )
            return true;

        // Up
        if (
            solveMaze(
                x - 1,
                y,
                maze,
                solution
            )
        )
            return true;

        // Left
        if (
            solveMaze(
                x,
                y - 1,
                maze,
                solution
            )
        )
            return true;

        solution[x][y] = 0;

        return false;
    }

    return false;
}

int main() {

    vector<vector<int>> maze = {
        {1, 0, 0, 0},
        {1, 1, 0, 1},
        {0, 1, 0, 0},
        {1, 1, 1, 1}
    };

    vector<vector<int>> solution(
        N,
        vector<int>(N, 0)
    );

    if (
        solveMaze(
            0,
            0,
            maze,
            solution
        )
    ) {

        cout << "Path Found:\n\n";

        for (
            int i = 0;
            i < N;
            i++
        ) {
            for (
                int j = 0;
                j < N;
                j++
            ) {
                cout
                    << solution[i][j]
                    << " ";
            }
            cout << endl;
        }

    } else {

        cout
            << "No Path Exists!"
            << endl;
    }

    return 0;
}