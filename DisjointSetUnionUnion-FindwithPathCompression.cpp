#include <iostream>
#include <vector>

using namespace std;

class DisjointSet {
private:
    vector<int> parent;
    vector<int> rank;

public:
    DisjointSet(int n) {
        parent.resize(n);
        rank.resize(n, 0);

        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    // Find with Path Compression
    int find(int node) {
        if (parent[node] == node) {
            return node;
        }

        return parent[node] = find(parent[node]);
    }

    // Union by Rank
    void unite(int u, int v) {
        int rootU = find(u);
        int rootV = find(v);

        if (rootU == rootV) {
            return;
        }

        if (rank[rootU] < rank[rootV]) {
            parent[rootU] = rootV;
        }
        else if (rank[rootU] > rank[rootV]) {
            parent[rootV] = rootU;
        }
        else {
            parent[rootV] = rootU;
            rank[rootU]++;
        }
    }

    // Check if connected
    bool isConnected(int u, int v) {
        return find(u) == find(v);
    }

    void printParents() {
        cout << "\nParent Array:\n";

        for (int i = 0; i < parent.size(); i++) {
            cout << "Node " << i
                 << " -> "
                 << find(i)
                 << endl;
        }
    }
};

int main() {

    int n = 7;

    DisjointSet dsu(n);

    cout << "Creating " << n
         << " separate sets...\n";

    dsu.unite(0, 1);
    dsu.unite(1, 2);

    dsu.unite(3, 4);

    dsu.unite(5, 6);

    dsu.printParents();

    cout << "\nConnectivity Checks:\n";

    cout << "0 and 2: "
         << (dsu.isConnected(0, 2)
                 ? "Connected"
                 : "Not Connected")
         << endl;

    cout << "0 and 4: "
         << (dsu.isConnected(0, 4)
                 ? "Connected"
                 : "Not Connected")
         << endl;

    cout << "\nUnion(2, 4)\n";

    dsu.unite(2, 4);

    dsu.printParents();

    cout << "\nAfter Union:\n";

    cout << "0 and 4: "
         << (dsu.isConnected(0, 4)
                 ? "Connected"
                 : "Not Connected")
         << endl;

    return 0;
}