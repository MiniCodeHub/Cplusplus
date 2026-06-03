#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Edge Structure
struct Edge {
    int source;
    int destination;
    int weight;

    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

// Disjoint Set Union
class DSU {

private:
    vector<int> parent;
    vector<int> rank;

public:

    DSU(int n) {
        parent.resize(n);
        rank.resize(n, 0);

        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    int find(int node) {

        if (parent[node] == node) {
            return node;
        }

        return parent[node] =
            find(parent[node]);
    }

    bool unite(int u, int v) {

        int rootU = find(u);
        int rootV = find(v);

        if (rootU == rootV) {
            return false;
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

        return true;
    }
};

int main() {

    int vertices = 6;

    vector<Edge> edges = {

        {0, 1, 4},
        {0, 2, 3},

        {1, 2, 1},
        {1, 3, 2},

        {2, 3, 4},
        {3, 4, 2},

        {4, 5, 6}
    };

    sort(
        edges.begin(),
        edges.end()
    );

    DSU dsu(vertices);

    vector<Edge> mst;

    int totalWeight = 0;

    for (const Edge& edge : edges) {

        if (
            dsu.unite(
                edge.source,
                edge.destination
            )
        ) {

            mst.push_back(edge);

            totalWeight += edge.weight;
        }
    }

    cout << "Minimum Spanning Tree\n\n";

    for (const Edge& edge : mst) {

        cout
            << edge.source
            << " -- "
            << edge.destination
            << " : "
            << edge.weight
            << endl;
    }

    cout
        << "\nTotal MST Weight: "
        << totalWeight
        << endl;

    return 0;
}