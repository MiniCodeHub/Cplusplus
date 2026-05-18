#include <iostream>
#include <vector>
#include <climits>

using namespace std;

class CoinChange {
public:

    int minimumCoins(
        vector<int>& coins,
        int amount
    ) {

        // DP array
        vector<int> dp(amount + 1, INT_MAX);

        // Base case
        dp[0] = 0;

        // Build DP table
        for (int i = 1; i <= amount; i++) {

            for (int coin : coins) {

                if (
                    coin <= i &&
                    dp[i - coin] != INT_MAX
                ) {

                    dp[i] = min(
                        dp[i],
                        dp[i - coin] + 1
                    );
                }
            }
        }

        // Impossible case
        if (dp[amount] == INT_MAX)
            return -1;

        return dp[amount];
    }

    void printDPArray(
        vector<int>& coins,
        int amount
    ) {

        vector<int> dp(amount + 1, INT_MAX);

        dp[0] = 0;

        for (int i = 1; i <= amount; i++) {

            for (int coin : coins) {

                if (
                    coin <= i &&
                    dp[i - coin] != INT_MAX
                ) {

                    dp[i] = min(
                        dp[i],
                        dp[i - coin] + 1
                    );
                }
            }
        }

        cout << "\nDP Array:\n\n";

        for (int i = 0; i <= amount; i++) {

            cout << "Amount "
                 << i
                 << " -> ";

            if (dp[i] == INT_MAX)
                cout << "INF";
            else
                cout << dp[i];

            cout << endl;
        }
    }
};

int main() {

    vector<int> coins = {
        1,
        2,
        5
    };

    int targetAmount = 11;

    CoinChange solver;

    cout << "Coins Available:\n";

    for (int coin : coins) {
        cout << coin << " ";
    }

    cout << "\n\nTarget Amount:\n";
    cout << targetAmount << endl;

    int result =
        solver.minimumCoins(
            coins,
            targetAmount
        );

    cout << "\nMinimum Coins Needed:\n";

    if (result == -1)
        cout << "Not Possible";
    else
        cout << result;

    cout << endl;

    solver.printDPArray(
        coins,
        targetAmount
    );

    return 0;
}