#include <iostream>
#include <vector>
#include <climits>

using namespace std;

class MatrixChainMultiplication {
private:
    vector<vector<int>> dp;

public:
    // Solve using Dynamic Programming
    int solve(vector<int>& dimensions) {

        int n = dimensions.size();

        // DP table
        dp.resize(n, vector<int>(n, 0));

        // Chain length
        for (int length = 2; length < n; length++) {

            for (int i = 1; i < n - length + 1; i++) {

                int j = i + length - 1;

                dp[i][j] = INT_MAX;

                // Try every split position
                for (int k = i; k < j; k++) {

                    int cost =
                        dp[i][k]
                        + dp[k + 1][j]
                        + dimensions[i - 1]
                          * dimensions[k]
                          * dimensions[j];

                    dp[i][j] = min(dp[i][j], cost);
                }
            }
        }

        return dp[1][n - 1];
    }

    // Print DP table
    void printTable(int size) {

        cout << "\nDP Table:\n\n";

        for (int i = 1; i < size; i++) {

            for (int j = 1; j < size; j++) {

                if (j < i)
                    cout << "-\t";
                else
                    cout << dp[i][j] << "\t";
            }

            cout << endl;
        }
    }
};

int main() {

    // Matrix dimensions
    // Example:
    // A1 = 10x30
    // A2 = 30x5
    // A3 = 5x60
    // A4 = 60x20

    vector<int> dimensions = {
        10,
        30,
        5,
        60,
        20
    };

    MatrixChainMultiplication mcm;

    cout << "Matrix Dimensions:\n\n";

    for (int i = 0; i < dimensions.size() - 1; i++) {

        cout
            << "A" << i + 1
            << " = "
            << dimensions[i]
            << "x"
            << dimensions[i + 1]
            << endl;
    }

    int minimumCost = mcm.solve(dimensions);

    cout << "\nMinimum Multiplication Cost:\n";
    cout << minimumCost << endl;

    mcm.printTable(dimensions.size());

    return 0;
}