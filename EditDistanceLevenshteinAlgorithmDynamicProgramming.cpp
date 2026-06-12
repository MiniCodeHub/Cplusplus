#include <iostream>
#include <vector>
#include <string>

using namespace std;

int editDistance(
    const string& word1,
    const string& word2
) {
    int n = word1.size();
    int m = word2.size();

    vector<vector<int>> dp(
        n + 1,
        vector<int>(m + 1, 0)
    );

    for (int i = 0; i <= n; i++) {
        dp[i][0] = i;
    }

    for (int j = 0; j <= m; j++) {
        dp[0][j] = j;
    }

    for (int i = 1; i <= n; i++) {

        for (int j = 1; j <= m; j++) {

            if (
                word1[i - 1] ==
                word2[j - 1]
            ) {

                dp[i][j] =
                    dp[i - 1][j - 1];
            }
            else {

                dp[i][j] =
                    1 +
                    min(
                        dp[i - 1][j],       // Delete
                        min(
                            dp[i][j - 1],   // Insert
                            dp[i - 1][j - 1]// Replace
                        )
                    );
            }
        }
    }

    return dp[n][m];
}

int main() {

    string word1 = "kitten";
    string word2 = "sitting";

    cout << "Word 1: "
         << word1 << endl;

    cout << "Word 2: "
         << word2 << endl;

    cout << "\nEdit Distance: "
         << editDistance(
                word1,
                word2
            )
         << endl;

    return 0;
}