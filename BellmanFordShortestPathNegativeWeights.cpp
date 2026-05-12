#include <iostream>
#include <vector>
#include <climits>

using namespace std;

// Edge structure
struct Edge {
    int source;
    int destination;
    int weight;
};

class Graph {
private:
    int vertices;
    vector<Edge> edges;

public:
    Graph(int v) {
        vertices = v;
    }

    // Add edge to graph
    void addEdge(int s, int d, int w) {
        edges.push_back({s, d, w});
    }

    // Bellman-Ford Algorithm
    void bellmanFord(int start) {

        vector<int> distance(vertices, INT_MAX);

        distance[start] = 0;

        // Relax all edges V-1 times
        for (int i = 1; i <= vertices - 1; i++) {

            for (Edge edge : edges) {

                int u = edge.source;
                int v = edge.destination;
                int w = edge.weight;

                if (
                    distance[u] != INT_MAX &&
                    distance[u] + w < distance[v]
                ) {
                    distance[v] = distance[u] + w;
                }
            }
        }

        // Check for negative weight cycle
        for (Edge edge : edges) {

            int u = edge.source;
            int v = edge.destination;
            int w = edge.weight;

            if (
                distance[u] != INT_MAX &&
                distance[u] + w < distance[v]
            ) {
                cout << "Negative weight cycle detected!" << endl;
                return;
            }
        }

        // Print shortest distances
        cout << "Shortest distances from node "
             << start
             << ":\n\n";

        for (int i = 0; i < vertices; i++) {

            cout << "Node " << i << " -> ";

            if (distance[i] == INT_MAX)
                cout << "INF";
            else
                cout << distance[i];

            cout << endl;
        }
    }
};

int main() {

    Graph graph(5);

    graph.addEdge(0, 1, 6);
    graph.addEdge(0, 2, 7);

    graph.addEdge(1, 2, 8);
    graph.addEdge(1, 3, 5);
    graph.addEdge(1, 4, -4);

    graph.addEdge(2, 3, -3);
    graph.addEdge(2, 4, 9);

    graph.addEdge(3, 1, -2);

    graph.addEdge(4, 0, 2);
    graph.addEdge(4, 3, 7);

    graph.bellmanFord(0);

    return 0;
}