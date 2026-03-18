#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <algorithm>
#include <iomanip>
#include <functional>
using namespace std;

// -- Generic Graph (directed or undirected, weighted or unweighted) ---------
class Graph {
private:
    int  vertices;
    bool directed;
    vector<vector<pair<int,int>>> adj; // adj[u] = {v, weight}

public:
    explicit Graph(int v, bool directed = false)
        : vertices(v), directed(directed), adj(v) {}

    void addEdge(int u, int v, int w = 1) {
        adj[u].push_back({v, w});
        if (!directed) adj[v].push_back({u, w});
    }

    // BFS - returns traversal order from src
    vector<int> bfs(int src) const {
        vector<bool> visited(vertices, false);
        vector<int>  order;
        queue<int>   q;
        visited[src] = true;
        q.push(src);
        while (!q.empty()) {
            int node = q.front(); q.pop();
            order.push_back(node);
            for (auto [nb, _] : adj[node]) {
                if (!visited[nb]) {
                    visited[nb] = true;
                    q.push(nb);
                }
            }
        }
        return order;
    }

    // BFS - shortest path distances from src (-1 = unreachable)
    vector<int> bfsDistances(int src) const {
        vector<int> dist(vertices, -1);
        queue<int>  q;
        dist[src] = 0;
        q.push(src);
        while (!q.empty()) {
            int node = q.front(); q.pop();
            for (auto [nb, _] : adj[node]) {
                if (dist[nb] == -1) {
                    dist[nb] = dist[node] + 1;
                    q.push(nb);
                }
            }
        }
        return dist;
    }

    // BFS - shortest path from src to dst (returns path nodes)
    vector<int> bfsPath(int src, int dst) const {
        vector<int> parent(vertices, -1);
        vector<bool> visited(vertices, false);
        queue<int> q;
        visited[src] = true;
        q.push(src);
        while (!q.empty()) {
            int node = q.front(); q.pop();
            if (node == dst) break;
            for (auto [nb, _] : adj[node]) {
                if (!visited[nb]) {
                    visited[nb]  = true;
                    parent[nb]   = node;
                    q.push(nb);
                }
            }
        }
        if (parent[dst] == -1 && src != dst) return {};
        vector<int> path;
        for (int cur = dst; cur != -1; cur = parent[cur]) path.push_back(cur);
        reverse(path.begin(), path.end());
        return path;
    }

    // DFS - recursive
    void dfsHelper(int node, vector<bool>& visited, vector<int>& order) const {
        visited[node] = true;
        order.push_back(node);
        for (auto [nb, _] : adj[node]) {
            if (!visited[nb]) dfsHelper(nb, visited, order);
        }
    }

    vector<int> dfs(int src) const {
        vector<bool> visited(vertices, false);
        vector<int>  order;
        dfsHelper(src, visited, order);
        return order;
    }

    // DFS - iterative (using explicit stack)
    vector<int> dfsIterative(int src) const {
        vector<bool> visited(vertices, false);
        vector<int>  order;
        stack<int>   st;
        st.push(src);
        while (!st.empty()) {
            int node = st.top(); st.pop();
            if (visited[node]) continue;
            visited[node] = true;
            order.push_back(node);
            for (auto it = adj[node].rbegin(); it != adj[node].rend(); ++it)
                if (!visited[it->first]) st.push(it->first);
        }
        return order;
    }

    // Connected components (for undirected graphs)
    int countComponents() const {
        vector<bool> visited(vertices, false);
        int count = 0;
        for (int i = 0; i < vertices; i++) {
            if (!visited[i]) {
                count++;
                // mini BFS
                queue<int> q; q.push(i); visited[i] = true;
                while (!q.empty()) {
                    int node = q.front(); q.pop();
                    for (auto [nb, _] : adj[node])
                        if (!visited[nb]) { visited[nb] = true; q.push(nb); }
                }
            }
        }
        return count;
    }

    // Cycle detection (undirected)
    bool hasCycleUndirected() const {
        vector<bool> visited(vertices, false);
        function<bool(int,int)> dfsCycle = [&](int node, int parent) -> bool {
            visited[node] = true;
            for (auto [nb, _] : adj[node]) {
                if (!visited[nb]) { if (dfsCycle(nb, node)) return true; }
                else if (nb != parent)  return true;
            }
            return false;
        };
        for (int i = 0; i < vertices; i++)
            if (!visited[i] && dfsCycle(i, -1)) return true;
        return false;
    }

    // Topological sort (Kahn's algorithm, for DAGs)
    vector<int> topologicalSort() const {
        if (!directed) { cout << "Topological sort requires directed graph.\n"; return {}; }
        vector<int> inDegree(vertices, 0);
        for (int u = 0; u < vertices; u++)
            for (auto [v, _] : adj[u]) inDegree[v]++;
        queue<int> q;
        for (int i = 0; i < vertices; i++) if (inDegree[i] == 0) q.push(i);
        vector<int> order;
        while (!q.empty()) {
            int node = q.front(); q.pop();
            order.push_back(node);
            for (auto [nb, _] : adj[node])
                if (--inDegree[nb] == 0) q.push(nb);
        }
        if ((int)order.size() != vertices) {
            cout << "Cycle detected - no topological order.\n";
            return {};
        }
        return order;
    }

    void printAdjList() const {
        cout << "Adjacency List (" << (directed ? "directed" : "undirected") << "):\n";
        for (int i = 0; i < vertices; i++) {
            cout << "  " << i << " -> [";
            for (size_t j = 0; j < adj[i].size(); j++) {
                cout << adj[i][j].first;
                if (adj[i][j].second != 1) cout << "(" << adj[i][j].second << ")";
                if (j + 1 < adj[i].size()) cout << ", ";
            }
            cout << "]\n";
        }
    }

    int numVertices() const { return vertices; }
};

void printVec(const vector<int>& v, const string& label) {
    cout << label << ": ";
    for (size_t i = 0; i < v.size(); i++) {
        cout << v[i];
        if (i + 1 < v.size()) cout << " -> ";
    }
    cout << "\n";
}

int main() {
    cout << "=== Graph: Adjacency List + BFS + DFS ===\n\n";

    // -- Undirected graph --
    cout << "-- Undirected Graph (7 nodes) --\n";
    Graph g(7, false);
    g.addEdge(0, 1); g.addEdge(0, 2);
    g.addEdge(1, 3); g.addEdge(1, 4);
    g.addEdge(2, 5); g.addEdge(2, 6);
    g.addEdge(3, 4);
    g.printAdjList();

    cout << "\n-- BFS from node 0 --\n";
    printVec(g.bfs(0), "BFS order");

    cout << "\n-- DFS from node 0 (recursive) --\n";
    printVec(g.dfs(0), "DFS recursive");

    cout << "\n-- DFS from node 0 (iterative) --\n";
    printVec(g.dfsIterative(0), "DFS iterative");

    cout << "\n-- BFS shortest distances from node 0 --\n";
    auto dist = g.bfsDistances(0);
    for (int i = 0; i < g.numVertices(); i++)
        cout << "  dist(0 -> " << i << ") = " << dist[i] << "\n";

    cout << "\n-- BFS shortest path: 0 -> 6 --\n";
    printVec(g.bfsPath(0, 6), "Path");

    cout << "\n-- Connected components: " << g.countComponents() << " --\n";
    cout << "-- Has cycle: " << (g.hasCycleUndirected() ? "Yes" : "No") << " --\n";

    // -- Directed graph / DAG for topological sort --
    cout << "\n-- Directed Acyclic Graph (6 nodes) --\n";
    Graph dag(6, true);
    dag.addEdge(5, 2); dag.addEdge(5, 0);
    dag.addEdge(4, 0); dag.addEdge(4, 1);
    dag.addEdge(2, 3); dag.addEdge(3, 1);
    dag.printAdjList();

    cout << "\n-- Topological Sort (Kahn's algorithm) --\n";
    printVec(dag.topologicalSort(), "Topo order");

    cout << "\n-- BFS from node 5 (directed) --\n";
    printVec(dag.bfs(5), "BFS order");

    cout << "\n-- DFS from node 5 (directed) --\n";
    printVec(dag.dfs(5), "DFS order");

    // -- Disconnected graph --
    cout << "\n-- Disconnected Graph (5 nodes) --\n";
    Graph disc(5, false);
    disc.addEdge(0, 1); disc.addEdge(1, 2);
    disc.addEdge(3, 4);
    disc.printAdjList();
    cout << "Connected components: " << disc.countComponents() << "\n";

    return 0;
}