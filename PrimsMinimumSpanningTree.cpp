#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct Edge {
    int weight;
    int node;

    bool operator>(const Edge& other) const {
        return weight > other.weight;
    }
};

class Graph {
private:
    int vertices;
    vector<vector<pair<int, int>>> adj;

public:
    Graph(int v) {
        vertices = v;
        adj.resize(v);
    }

    void addEdge(
        int u,
        int v,
        int weight
    ) {
        adj[u].push_back({v, weight});
        adj[v].push_back({u, weight});
    }

    void primMST() {

        priority_queue<
            Edge,
            vector<Edge>,
            greater<Edge>
        > pq;

        vector<bool> visited(
            vertices,
            false
        );

        int totalWeight = 0;

        vector<pair<int, int>> mstEdges;

        pq.push({0, 0});

        while (!pq.empty()) {

            Edge current = pq.top();
            pq.pop();

            int node = current.node;
            int weight = current.weight;

            if (visited[node]) {
                continue;
            }

            visited[node] = true;

            totalWeight += weight;

            for (
                auto& neighbor :
                adj[node]
            ) {

                int nextNode =
                    neighbor.first;

                int nextWeight =
                    neighbor.second;

                if (
                    !visited[nextNode]
                ) {

                    pq.push(
                        {
                            nextWeight,
                            nextNode
                        }
                    );
                }
            }
        }

        cout
            << "Total MST Weight: "
            << totalWeight
            << endl;
    }
};

int main() {

    Graph graph(6);

    graph.addEdge(0, 1, 4);
    graph.addEdge(0, 2, 3);
    graph.addEdge(1, 2, 1);
    graph.addEdge(1, 3, 2);
    graph.addEdge(2, 3, 4);
    graph.addEdge(3, 4, 2);
    graph.addEdge(4, 5, 6);

    cout
        << "Prim's Minimum Spanning Tree\n\n";

    graph.primMST();

    return 0;
}