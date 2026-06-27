#include <iostream>
#include <vector>

using namespace std;

// ----------------------------
// Backtracking Solution
// ----------------------------
bool subsetSumBacktracking(
    vector<int>& nums,
    int index,
    int target
) {

    if (target == 0)
        return true;

    if (
        index == nums.size() ||
        target < 0
    )
        return false;

    // Include current element
    if (
        subsetSumBacktracking(
            nums,
            index + 1,
            target - nums[index]
        )
    )
        return true;

    // Exclude current element
    return subsetSumBacktracking(
        nums,
        index + 1,
        target
    );
}

// ----------------------------
// Dynamic Programming Solution
// ----------------------------
bool subsetSumDP(
    vector<int>& nums,
    int target
) {

    int n = nums.size();

    vector<vector<bool>> dp(
        n + 1,
        vector<bool>(
            target + 1,
            false
        )
    );

    for (int i = 0; i <= n; i++)
        dp[i][0] = true;

    for (
        int i = 1;
        i <= n;
        i++
    ) {

        for (
            int sum = 1;
            sum <= target;
            sum++
        ) {

            if (
                nums[i - 1] <= sum
            ) {

                dp[i][sum] =
                    dp[i - 1][sum] ||
                    dp[i - 1][
                        sum - nums[i - 1]
                    ];

            } else {

                dp[i][sum] =
                    dp[i - 1][sum];
            }
        }
    }

    return dp[n][target];
}

int main() {

    vector<int> nums = {
        3,
        34,
        4,
        12,
        5,
        2
    };

    int target = 9;

    cout << "Array: ";

    for (int num : nums)
        cout << num << " ";

    cout << "\nTarget Sum: "
         << target
         << "\n\n";

    bool backtracking =
        subsetSumBacktracking(
            nums,
            0,
            target
        );

    bool dp =
        subsetSumDP(
            nums,
            target
        );

    cout
        << "Backtracking: ";

    if (backtracking)
        cout << "Subset Exists\n";
    else
        cout << "Subset Doesn't Exist\n";

    cout
        << "Dynamic Programming: ";

    if (dp)
        cout << "Subset Exists\n";
    else
        cout << "Subset Doesn't Exist\n";

    return 0;
}