#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int longestIncreasingSubsequence(
    const vector<int>& nums
) {
    vector<int> lis;

    for (int num : nums) {

        auto position =
            lower_bound(
                lis.begin(),
                lis.end(),
                num
            );

        if (position == lis.end()) {

            lis.push_back(num);
        }
        else {

            *position = num;
        }
    }

    return lis.size();
}

int main() {

    vector<int> nums = {
        10,
        9,
        2,
        5,
        3,
        7,
        101,
        18
    };

    cout << "Array:\n";

    for (int num : nums) {
        cout << num << " ";
    }

    cout << "\n\n";

    int lisLength =
        longestIncreasingSubsequence(nums);

    cout
        << "Length of Longest Increasing Subsequence: "
        << lisLength
        << endl;

    return 0;
}