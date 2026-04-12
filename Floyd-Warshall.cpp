#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <stdexcept>
#include <limits>
using namespace std;

const double INF = numeric_limits<double>::infinity();

// -- Floyd-Warshall ---------------------------------------------------------
class FloydWarshall {
private:
    int              n;
    vector<string>   labels;
    vector<vector<double>> dist;   // shortest distances
    vector<vector<int>>    next;   // next-hop for path reconstruction

public:
    explicit FloydWarshall(int vertices, vector<string> lbls = {})
        : n(vertices), labels(lbls),
          dist(vertices, vector<double>(vertices, INF)),
          next(vertices, vector<int>(vertices, -1))
    {
        if (labels.empty()) for (int i = 0; i < n; i++) labels.push_back(to_string(i));
        for (int i = 0; i < n; i++) { dist[i][i] = 0; next[i][i] = i; }
    }

    // Add a directed edge (use twice for undirected)
    void addEdge(int u, int v, double w) {
        if (u < 0 || u >= n || v < 0 || v >= n) throw out_of_range("Vertex out of range.");
        if (w < dist[u][v]) {           // keep lightest parallel edge
            dist[u][v] = w;
            next[u][v] = v;
        }
    }

    void addUndirectedEdge(int u, int v, double w) {
        addEdge(u, v, w);
        addEdge(v, u, w);
    }

    // Run the algorithm: O(V^3)
    void solve() {
        for (int k = 0; k < n; k++) {
            for (int i = 0; i < n; i++) {
                if (dist[i][k] == INF) continue;
                for (int j = 0; j < n; j++) {
                    if (dist[k][j] == INF) continue;
                    double through = dist[i][k] + dist[k][j];
                    if (through < dist[i][j]) {
                        dist[i][j] = through;
                        next[i][j] = next[i][k];
                    }
                }
            }
        }
    }

    // Detect negative cycles (dist[i][i] < 0 after solve)
    bool hasNegativeCycle() const {
        for (int i = 0; i < n; i++) if (dist[i][i] < 0) return true;
        return false;
    }

    // Get shortest distance between two nodes
    double getDistance(int u, int v) const { return dist[u][v]; }

    // Reconstruct path from u to v
    vector<int> getPath(int u, int v) const {
        if (dist[u][v] == INF) return {};
        vector<int> path;
        for (int cur = u; cur != v; cur = next[cur][v]) {
            if (cur == -1) return {};
            path.push_back(cur);
        }
        path.push_back(v);
        return path;
    }

    void printPath(int u, int v) const {
        cout << labels[u] << " -> " << labels[v] << ": ";
        if (dist[u][v] == INF) { cout << "UNREACHABLE\n"; return; }
        auto path = getPath(u, v);
        for (size_t i = 0; i < path.size(); i++) {
            cout << labels[path[i]];
            if (i+1 < path.size()) cout << " -> ";
        }
        cout << "  (cost=" << fixed << setprecision(1) << dist[u][v] << ")\n";
    }

    // Print full distance matrix
    void printDistMatrix() const {
        const int W = 9;
        cout << string(W, ' ');
        for (const auto& l : labels) cout << setw(W) << l;
        cout << "\n" << string(W + W*n, '-') << "\n";
        for (int i = 0; i < n; i++) {
            cout << setw(W) << labels[i];
            for (int j = 0; j < n; j++) {
                if (dist[i][j] == INF) cout << setw(W) << "INF";
                else cout << setw(W) << fixed << setprecision(1) << dist[i][j];
            }
            cout << "\n";
        }
    }

    // Print next-hop matrix
    void printNextMatrix() const {
        const int W = 6;
        cout << string(W, ' ');
        for (const auto& l : labels) cout << setw(W) << l;
        cout << "\n" << string(W + W*n, '-') << "\n";
        for (int i = 0; i < n; i++) {
            cout << setw(W) << labels[i];
            for (int j = 0; j < n; j++) {
                if (next[i][j] == -1) cout << setw(W) << "-";
                else cout << setw(W) << labels[next[i][j]];
            }
            cout << "\n";
        }
    }

    // Diameter: longest shortest path
    double diameter() const {
        double d = 0;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (dist[i][j] != INF && dist[i][j] > d) d = dist[i][j];
        return d;
    }

    // Center: node(s) with minimum eccentricity
    vector<int> center() const {
        vector<double> ecc(n, 0);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (dist[i][j] != INF) ecc[i] = max(ecc[i], dist[i][j]);
        double minEcc = *min_element(ecc.begin(), ecc.end());
        vector<int> centers;
        for (int i = 0; i < n; i++) if (ecc[i] == minEcc) centers.push_back(i);
        return centers;
    }

    // Transitive closure (reachability)
    vector<vector<bool>> transitiveClosure() const {
        vector<vector<bool>> reach(n, vector<bool>(n, false));
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (dist[i][j] != INF) reach[i][j] = true;
        return reach;
    }

    int size() const { return n; }
    const vector<string>& getLabels() const { return labels; }
};

void sep(const string& t) {
    cout << "\n" << string(60, '=') << "\n " << t << "\n" << string(60, '=') << "\n";
}

int main() {
    cout << "=== Floyd-Warshall All-Pairs Shortest Path ===\n";

    // -- Demo 1: Simple directed graph -------------------------------------
    sep("1. Directed Weighted Graph (4 nodes)");
    FloydWarshall fw1(4, {"A","B","C","D"});
    fw1.addEdge(0,1,3); fw1.addEdge(0,3,7);
    fw1.addEdge(1,0,8); fw1.addEdge(1,2,2);
    fw1.addEdge(2,0,5); fw1.addEdge(2,3,1);
    fw1.addEdge(3,0,2);
    fw1.solve();

    cout << "\nDistance Matrix:\n";
    fw1.printDistMatrix();

    cout << "\nNext-Hop Matrix:\n";
    fw1.printNextMatrix();

    cout << "\nAll Paths:\n";
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (i != j) fw1.printPath(i, j);

    cout << "\nDiameter: " << fw1.diameter() << "\n";
    auto centers = fw1.center();
    cout << "Center(s): ";
    for (int c : centers) cout << fw1.getLabels()[c] << " ";
    cout << "\n";

    // -- Demo 2: City road network ------------------------------------------
    sep("2. City Road Network (Undirected)");
    FloydWarshall fw2(6, {"NYC","LAX","CHI","MIA","SEA","DEN"});
    fw2.addUndirectedEdge(0,1,2800); fw2.addUndirectedEdge(0,2,790);
    fw2.addUndirectedEdge(0,3,1280); fw2.addUndirectedEdge(1,4,1140);
    fw2.addUndirectedEdge(1,5,1020); fw2.addUndirectedEdge(2,4,2050);
    fw2.addUndirectedEdge(2,5,920);  fw2.addUndirectedEdge(3,0,1280);
    fw2.addUndirectedEdge(4,5,1310); fw2.addUndirectedEdge(2,3,1300);
    fw2.solve();

    cout << "\nDistance Matrix (miles):\n";
    fw2.printDistMatrix();

    cout << "\nShortest routes from NYC:\n";
    for (int j = 1; j < 6; j++) fw2.printPath(0, j);

    cout << "\nNetwork diameter: " << fw2.diameter() << " miles\n";
    auto c2 = fw2.center();
    cout << "Network center(s): ";
    for (int c : c2) cout << fw2.getLabels()[c] << " ";
    cout << "\n";

    // -- Demo 3: Negative weights (NO negative cycle) -----------------------
    sep("3. Graph with Negative Weights (no cycle)");
    FloydWarshall fw3(4, {"P","Q","R","S"});
    fw3.addEdge(0,1,1);  fw3.addEdge(0,2,4);
    fw3.addEdge(1,2,2);  fw3.addEdge(1,3,5);
    fw3.addEdge(2,3,-1); fw3.addEdge(3,0,3);
    fw3.solve();
    cout << "Negative cycle: " << (fw3.hasNegativeCycle() ? "YES (invalid)" : "NO (valid)") << "\n\n";
    fw3.printDistMatrix();

    // -- Demo 4: Detect negative cycle ------------------------------------
    sep("4. Graph with Negative Cycle (detection)");
    FloydWarshall fw4(3, {"X","Y","Z"});
    fw4.addEdge(0,1,4); fw4.addEdge(1,2,-6); fw4.addEdge(2,0,1);
    fw4.solve();
    cout << "Negative cycle detected: " << (fw4.hasNegativeCycle() ? "YES" : "NO") << "\n";

    // -- Demo 5: Transitive closure ----------------------------------------
    sep("5. Transitive Closure");
    FloydWarshall fw5(4, {"1","2","3","4"});
    fw5.addEdge(0,1,1); fw5.addEdge(1,2,1); fw5.addEdge(2,3,1);
    fw5.solve();
    auto reach = fw5.transitiveClosure();
    cout << "Reachability matrix:\n";
    cout << string(6,' ');
    for (const auto& l : fw5.getLabels()) cout << setw(4) << l;
    cout << "\n";
    for (int i = 0; i < 4; i++) {
        cout << setw(6) << fw5.getLabels()[i];
        for (int j = 0; j < 4; j++) cout << setw(4) << (reach[i][j] ? "1" : "0");
        cout << "\n";
    }

    // -- Demo 6: Disconnected graph ----------------------------------------
    sep("6. Disconnected Graph");
    FloydWarshall fw6(4, {"A","B","C","D"});
    fw6.addUndirectedEdge(0,1,5);
    fw6.addUndirectedEdge(2,3,3);
    fw6.solve();
    fw6.printDistMatrix();
    cout << "\nA->C: "; fw6.printPath(0,2);
    cout << "A->D: "; fw6.printPath(0,3);

    return 0;
}