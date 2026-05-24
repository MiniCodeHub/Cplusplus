#include <iostream>
#include <vector>

using namespace std;

class FenwickTree {

private:

    vector<int> bit;
    int size;

public:

    // Constructor
    FenwickTree(int n) {

        size = n;

        bit.resize(n + 1, 0);
    }

    // Update Value
    void update(int index, int value) {

        while (index <= size) {

            bit[index] += value;

            index += index & (-index);
        }
    }

    // Get Prefix Sum
    int query(int index) {

        int sum = 0;

        while (index > 0) {

            sum += bit[index];

            index -= index & (-index);
        }

        return sum;
    }

    // Range Sum Query
    int rangeQuery(int left, int right) {

        return query(right)
             - query(left - 1);
    }

    // Print Tree
    void printTree() {

        cout << "\nFenwick Tree Array:\n\n";

        for (int i = 1; i <= size; i++) {

            cout << bit[i] << " ";
        }

        cout << endl;
    }
};

int main() {

    vector<int> arr = {
        0,
        3,
        2,
        -1,
        6,
        5,
        4,
        -3,
        3,
        7,
        2,
        3
    };

    int n = arr.size() - 1;

    FenwickTree tree(n);

    // Build Tree
    for (int i = 1; i <= n; i++) {

        tree.update(i, arr[i]);
    }

    cout << "Original Array:\n\n";

    for (int i = 1; i <= n; i++) {

        cout << arr[i] << " ";
    }

    cout << endl;

    tree.printTree();

    // Prefix Sum
    cout << "\nPrefix Sum till index 5:\n";

    cout << tree.query(5) << endl;

    // Range Sum
    cout << "\nRange Sum (3 to 8):\n";

    cout << tree.rangeQuery(3, 8)
         << endl;

    // Update
    cout << "\nUpdating index 4 by +5\n";

    tree.update(4, 5);

    tree.printTree();

    cout << "\nNew Prefix Sum till index 5:\n";

    cout << tree.query(5)
         << endl;

    return 0;
}