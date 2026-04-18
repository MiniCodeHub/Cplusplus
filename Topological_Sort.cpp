#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <queue>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <stdexcept>
#include <iomanip>
#include <functional>
using namespace std;

// -- Directed Graph ---------------------------------------------------------
class DiGraph {
private:
    int              n;
    vector<string>   labels;
    vector<vector<int>> adj;
    vector<vector<int>> radj;  // reverse adjacency (for in-degree / Kahn's)

public:
    explicit DiGraph(int v, vector<string> lbls = {})
        : n(v), adj(v), radj(v)
    {
        labels = lbls.empty()
            ? vector<string>(v, "")
            : lbls;
        for (int i = 0; i < v && labels[i].empty(); i++)
            labels[i] = to_string(i);
    }

    void addEdge(int u, int v) {
        if (u < 0 || u >= n || v < 0 || v >= n) throw out_of_range("Vertex index out of range.");
        adj[u].push_back(v);
        radj[v].push_back(u);
    }

    int  numV()            const { return n; }
    const string& label(int v) const { return labels[v]; }
    const vector<int>& neighbors(int v) const { return adj[v]; }

    void printAdjList() const {
        cout << "Adjacency List (directed):\n";
        for (int u = 0; u < n; u++) {
            cout << "  " << setw(12) << labels[u] << " -> ";
            for (int v : adj[u]) cout << labels[v] << "  ";
            cout << "\n";
        }
    }

    // -- DFS-based Topological Sort -----------------------------------------
    // Returns empty vector if graph has a cycle
    vector<int> topoSortDFS() const {
        vector<int>  color(n, 0);  // 0=white, 1=gray(in-stack), 2=black(done)
        stack<int>   result;
        bool         hasCycle = false;

        function<void(int)> dfs = [&](int u) {
            if (hasCycle) return;
            color[u] = 1;
            for (int v : adj[u]) {
                if (color[v] == 1) { hasCycle = true; return; }
                if (color[v] == 0) dfs(v);
            }
            color[u] = 2;
            result.push(u);
        };

        for (int i = 0; i < n; i++) if (color[i] == 0) dfs(i);
        if (hasCycle) return {};

        vector<int> order;
        while (!result.empty()) { order.push_back(result.top()); result.pop(); }
        return order;
    }

    // -- Kahn's Algorithm (BFS) ---------------------------------------------
    // Also detects cycles; returns empty if cycle found
    vector<int> topoSortKahn() const {
        vector<int> inDeg(n, 0);
        for (int u = 0; u < n; u++)
            for (int v : adj[u]) inDeg[v]++;

        queue<int> q;
        for (int i = 0; i < n; i++) if (inDeg[i] == 0) q.push(i);

        vector<int> order;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            order.push_back(u);
            for (int v : adj[u]) if (--inDeg[v] == 0) q.push(v);
        }
        return (int)order.size() == n ? order : vector<int>{};
    }

    // -- Detect cycle (DFS) -------------------------------------------------
    bool hasCycle() const {
        vector<int> color(n, 0);
        bool found = false;
        function<void(int)> dfs = [&](int u) {
            if (found) return;
            color[u] = 1;
            for (int v : adj[u]) {
                if (color[v] == 1) { found = true; return; }
                if (color[v] == 0) dfs(v);
            }
            color[u] = 2;
        };
        for (int i = 0; i < n && !found; i++) if (color[i] == 0) dfs(i);
        return found;
    }

    // -- Find all cycles ----------------------------------------------------
    vector<vector<int>> findCycles() const {
        vector<vector<int>> cycles;
        vector<int> color(n, 0);
        vector<int> path;

        function<void(int)> dfs = [&](int u) {
            color[u] = 1;
            path.push_back(u);
            for (int v : adj[u]) {
                if (color[v] == 1) {
                    // Found back edge u->v; extract cycle
                    auto it = find(path.begin(), path.end(), v);
                    if (it != path.end())
                        cycles.push_back(vector<int>(it, path.end()));
                } else if (color[v] == 0) {
                    dfs(v);
                }
            }
            path.pop_back();
            color[u] = 2;
        };

        for (int i = 0; i < n; i++) if (color[i] == 0) dfs(i);
        return cycles;
    }

    // -- Longest path (for DAGs) --------------------------------------------
    int longestPath() const {
        auto order = topoSortDFS();
        if (order.empty()) return -1;  // has cycle
        vector<int> dp(n, 0);
        for (int u : order)
            for (int v : adj[u])
                dp[v] = max(dp[v], dp[u] + 1);
        return *max_element(dp.begin(), dp.end());
    }

    // -- Earliest/Latest completion times (critical path) ------------------
    pair<vector<int>,vector<int>> criticalPath(const vector<int>& duration) const {
        auto order = topoSortDFS();
        if (order.empty()) return {{},{}};

        vector<int> earliest(n, 0);
        for (int u : order)
            for (int v : adj[u])
                earliest[v] = max(earliest[v], earliest[u] + duration[u]);

        int maxTime = *max_element(earliest.begin(), earliest.end());
        // Add durations to get completion
        for (int& e : earliest) {}  // already correct

        vector<int> latest(n, maxTime);
        for (int i = (int)order.size()-1; i >= 0; i--) {
            int u = order[i];
            for (int v : adj[u])
                latest[u] = min(latest[u], latest[v] - duration[u]);
        }
        return {earliest, latest};
    }
};

void printOrder(const vector<int>& order, const DiGraph& g, const string& label) {
    cout << label << ": ";
    if (order.empty()) { cout << "(cycle detected - no valid order)\n"; return; }
    for (size_t i = 0; i < order.size(); i++) {
        cout << g.label(order[i]);
        if (i + 1 < order.size()) cout << " -> ";
    }
    cout << "\n";
}

void sep(const string& t) {
    cout << "\n" << string(56, '=') << "\n " << t << "\n" << string(56, '=') << "\n";
}

int main() {
    cout << "=== Topological Sort (DFS + Kahn's) ===\n";

    // -- Demo 1: Classic DAG ------------------------------------------------
    sep("1. Classic DAG (6 nodes)");
    DiGraph g1(6, {"A","B","C","D","E","F"});
    g1.addEdge(5,2); g1.addEdge(5,0);
    g1.addEdge(4,0); g1.addEdge(4,1);
    g1.addEdge(2,3); g1.addEdge(3,1);
    g1.printAdjList();
    cout << "\nHas cycle: " << (g1.hasCycle() ? "YES" : "NO") << "\n\n";
    printOrder(g1.topoSortDFS(),  g1, "DFS-based ");
    printOrder(g1.topoSortKahn(), g1, "Kahn's BFS");

    // -- Demo 2: Build system (tasks with prerequisites) --------------------
    sep("2. Build System Dependencies");
    DiGraph build(9, {
        "install_deps","compile_proto","compile_core","compile_ui",
        "run_tests","build_assets","link_binary","package","deploy"
    });
    build.addEdge(0,1); build.addEdge(0,2); build.addEdge(0,3);
    build.addEdge(1,2); build.addEdge(2,4); build.addEdge(3,4);
    build.addEdge(3,5); build.addEdge(2,6); build.addEdge(5,6);
    build.addEdge(4,7); build.addEdge(6,7); build.addEdge(7,8);
    build.printAdjList();
    cout << "\n";
    printOrder(build.topoSortDFS(),  build, "Build order (DFS)  ");
    printOrder(build.topoSortKahn(), build, "Build order (Kahn's)");

    // -- Demo 3: Course prerequisites --------------------------------------
    sep("3. Course Prerequisites");
    DiGraph courses(7, {
        "Maths","Algorithms","Data Structures","Systems","Networking",
        "OS","Capstone"
    });
    courses.addEdge(0,1); courses.addEdge(0,2);
    courses.addEdge(1,6); courses.addEdge(2,6);
    courses.addEdge(3,5); courses.addEdge(3,4);
    courses.addEdge(4,6); courses.addEdge(5,6);
    courses.printAdjList();
    cout << "\n";
    printOrder(courses.topoSortDFS(), courses, "Study plan");

    // -- Demo 4: Cycle detection --------------------------------------------
    sep("4. Cycle Detection");
    DiGraph cyclic(4, {"W","X","Y","Z"});
    cyclic.addEdge(0,1); cyclic.addEdge(1,2);
    cyclic.addEdge(2,3); cyclic.addEdge(3,1);  // cycle: X->Y->Z->X
    cout << "Graph: W->X->Y->Z->X\n";
    cout << "Has cycle: " << (cyclic.hasCycle() ? "YES" : "NO") << "\n\n";
    printOrder(cyclic.topoSortDFS(),  cyclic, "DFS sort ");
    printOrder(cyclic.topoSortKahn(), cyclic, "Kahn sort");
    auto cycles = cyclic.findCycles();
    cout << "\nCycles found:\n";
    for (auto& cycle : cycles) {
        cout << "  ";
        for (size_t i = 0; i < cycle.size(); i++)
            cout << cyclic.label(cycle[i]) << (i+1<cycle.size()?" -> ":"");
        cout << " -> (back)\n";
    }

    // -- Demo 5: Longest path (project scheduling) --------------------------
    sep("5. Longest Path in DAG");
    DiGraph proj(6, {"Start","A","B","C","D","End"});
    proj.addEdge(0,1); proj.addEdge(0,2);
    proj.addEdge(1,3); proj.addEdge(2,3);
    proj.addEdge(1,4); proj.addEdge(3,5); proj.addEdge(4,5);
    cout << "Longest path length: " << proj.longestPath() << " edges\n";

    // -- Demo 6: Critical path ----------------------------------------------
    sep("6. Critical Path Analysis");
    DiGraph cpa(6, {"Start","Design","Develop","Test","Review","Deploy"});
    cpa.addEdge(0,1); cpa.addEdge(0,2);
    cpa.addEdge(1,3); cpa.addEdge(2,3);
    cpa.addEdge(3,4); cpa.addEdge(4,5);
    vector<int> durations = {0, 5, 8, 4, 3, 2};
    auto [early, late] = cpa.criticalPath(durations);
    cout << left << setw(14) << "Task" << setw(10) << "Duration" << setw(10) << "Earliest" << "Latest\n";
    cout << string(44, '-') << "\n";
    for (int i = 0; i < 6; i++) {
        cout << setw(14) << cpa.label(i)
             << setw(10) << durations[i]
             << setw(10) << early[i]
             << late[i] << "\n";
    }

    return 0;
}