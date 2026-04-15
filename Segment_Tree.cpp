#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <functional>
#include <stdexcept>
#include <numeric>
#include <climits>
using namespace std;

// -- Generic Segment Tree ---------------------------------------------------
// Supports: range query, point update, range update (lazy propagation)
template<typename T>
class SegmentTree {
private:
    int           n;
    vector<T>     tree;
    vector<T>     lazy;
    function<T(T,T)> combine;
    T             identity;

    void build(const vector<T>& arr, int node, int start, int end) {
        if (start == end) {
            tree[node] = arr[start];
            return;
        }
        int mid = (start + end) / 2;
        build(arr, 2*node,   start,   mid);
        build(arr, 2*node+1, mid+1,   end);
        tree[node] = combine(tree[2*node], tree[2*node+1]);
    }

    void pushDown(int node, int start, int end) {
        if (lazy[node] == 0) return;
        int mid = (start + end) / 2;
        // Update children
        tree[2*node]   += lazy[node] * (mid - start + 1);
        tree[2*node+1] += lazy[node] * (end - mid);
        lazy[2*node]   += lazy[node];
        lazy[2*node+1] += lazy[node];
        lazy[node] = 0;
    }

    void updatePoint(int node, int start, int end, int idx, T val) {
        if (start == end) { tree[node] = val; return; }
        if (lazy[node]) pushDown(node, start, end);
        int mid = (start + end) / 2;
        if (idx <= mid) updatePoint(2*node,   start, mid,   idx, val);
        else            updatePoint(2*node+1, mid+1, end,   idx, val);
        tree[node] = combine(tree[2*node], tree[2*node+1]);
    }

    void updateRange(int node, int start, int end, int l, int r, T val) {
        if (r < start || end < l) return;
        if (l <= start && end <= r) {
            tree[node] += val * (end - start + 1);
            lazy[node] += val;
            return;
        }
        pushDown(node, start, end);
        int mid = (start + end) / 2;
        updateRange(2*node,   start, mid,   l, r, val);
        updateRange(2*node+1, mid+1, end,   l, r, val);
        tree[node] = combine(tree[2*node], tree[2*node+1]);
    }

    T query(int node, int start, int end, int l, int r) {
        if (r < start || end < l) return identity;
        if (l <= start && end <= r) return tree[node];
        if (lazy[node]) pushDown(node, start, end);
        int mid = (start + end) / 2;
        return combine(
            query(2*node,   start, mid,   l, r),
            query(2*node+1, mid+1, end,   l, r)
        );
    }

public:
    SegmentTree(const vector<T>& arr, function<T(T,T)> comb, T ident)
        : n((int)arr.size()), tree(4*arr.size(), ident),
          lazy(4*arr.size(), T{}), combine(comb), identity(ident)
    {
        if (!arr.empty()) build(arr, 1, 0, n-1);
    }

    // Point update: set index i to val
    void updatePoint(int i, T val) {
        if (i < 0 || i >= n) throw out_of_range("Index out of range.");
        updatePoint(1, 0, n-1, i, val);
    }

    // Range add: add val to every element in [l, r]
    void addRange(int l, int r, T val) {
        if (l < 0 || r >= n || l > r) throw out_of_range("Range out of bounds.");
        updateRange(1, 0, n-1, l, r, val);
    }

    // Range query: combine over [l, r]
    T query(int l, int r) {
        if (l < 0 || r >= n || l > r) throw out_of_range("Range out of bounds.");
        return query(1, 0, n-1, l, r);
    }

    int size() const { return n; }
};

// -- Specialised: Range Min/Max (no lazy needed for point-update only) ------
class RangeMinMax {
    int n;
    vector<int> minTree, maxTree;

    void build(const vector<int>& arr, int node, int s, int e) {
        if (s == e) { minTree[node] = maxTree[node] = arr[s]; return; }
        int m = (s+e)/2;
        build(arr, 2*node,   s,   m);
        build(arr, 2*node+1, m+1, e);
        minTree[node] = min(minTree[2*node], minTree[2*node+1]);
        maxTree[node] = max(maxTree[2*node], maxTree[2*node+1]);
    }

    void update(int node, int s, int e, int i, int v) {
        if (s == e) { minTree[node] = maxTree[node] = v; return; }
        int m = (s+e)/2;
        if (i <= m) update(2*node,   s,   m, i, v);
        else        update(2*node+1, m+1, e, i, v);
        minTree[node] = min(minTree[2*node], minTree[2*node+1]);
        maxTree[node] = max(maxTree[2*node], maxTree[2*node+1]);
    }

    pair<int,int> query(int node, int s, int e, int l, int r) {
        if (r < s || e < l) return {INT_MAX, INT_MIN};
        if (l <= s && e <= r) return {minTree[node], maxTree[node]};
        int m = (s+e)/2;
        auto [lmin,lmax] = query(2*node,   s,   m, l, r);
        auto [rmin,rmax] = query(2*node+1, m+1, e, l, r);
        return {min(lmin,rmin), max(lmax,rmax)};
    }

public:
    explicit RangeMinMax(const vector<int>& arr)
        : n(arr.size()), minTree(4*arr.size()), maxTree(4*arr.size()) {
        build(arr, 1, 0, n-1);
    }
    void update(int i, int v) { update(1, 0, n-1, i, v); }
    pair<int,int> query(int l, int r) { return query(1, 0, n-1, l, r); }
};

// -- Utilities --------------------------------------------------------------
void printArr(const vector<int>& a, const string& label = "") {
    if (!label.empty()) cout << label << ": ";
    cout << "[";
    for (int i = 0; i < (int)a.size(); i++) cout << a[i] << (i+1<(int)a.size()?", ":"");
    cout << "]\n";
}

void sep(const string& t) {
    cout << "\n" << string(56, '=') << "\n " << t << "\n" << string(56, '=') << "\n";
}

int main() {
    cout << "=== Segment Tree: Range Sum Query ===\n";

    vector<int> arr = {1, 3, 5, 7, 9, 11, 13, 15};
    printArr(arr, "Array");

    // -- Sum segment tree --------------------------------------------------
    sep("1. Range Sum Queries");
    SegmentTree<long long> sumTree(
        vector<long long>(arr.begin(), arr.end()),
        [](long long a, long long b){ return a + b; }, 0LL
    );

    auto testSum = [&](int l, int r) {
        cout << "  sum[" << l << ".." << r << "] = " << sumTree.query(l, r) << "\n";
    };

    testSum(0, 7);   // full array
    testSum(1, 4);   // [3,5,7,9] = 24
    testSum(2, 6);   // [5,7,9,11,13] = 45
    testSum(0, 0);   // single element
    testSum(7, 7);   // last element

    sep("2. Point Update");
    cout << "Before: sum[1..4] = " << sumTree.query(1, 4) << "\n";
    cout << "Update index 2: 5 -> 100\n";
    sumTree.updatePoint(2, 100);
    cout << "After:  sum[1..4] = " << sumTree.query(1, 4) << "\n";
    cout << "        sum[0..7] = " << sumTree.query(0, 7) << "\n";

    sep("3. Range Add (Lazy Propagation)");
    vector<long long> arr2(8, 0);
    SegmentTree<long long> lazyTree(arr2, [](long long a, long long b){ return a+b; }, 0LL);

    cout << "Initial: sum[0..7] = " << lazyTree.query(0, 7) << "\n";
    lazyTree.addRange(0, 7, 5);
    cout << "addRange(0,7,+5): sum[0..7] = " << lazyTree.query(0, 7) << "\n";
    lazyTree.addRange(2, 5, 10);
    cout << "addRange(2,5,+10): sum[2..5] = " << lazyTree.query(2, 5) << "\n";
    cout << "                   sum[0..7] = " << lazyTree.query(0, 7) << "\n";
    lazyTree.addRange(3, 3, -3);
    cout << "addRange(3,3,-3):  sum[3..3] = " << lazyTree.query(3, 3) << "\n";

    sep("4. Range Min/Max");
    vector<int> arr3 = {4, 2, 7, 1, 9, 3, 8, 5};
    printArr(arr3, "Array");
    RangeMinMax mmTree(arr3);

    auto testMM = [&](int l, int r) {
        auto [mn,mx] = mmTree.query(l, r);
        cout << "  [" << l << ".." << r << "] -> min=" << mn << "  max=" << mx << "\n";
    };

    testMM(0, 7);
    testMM(1, 5);
    testMM(3, 3);
    testMM(2, 6);

    cout << "\nUpdate index 4: 9 -> 0\n";
    mmTree.update(4, 0);
    testMM(0, 7);
    testMM(3, 5);

    sep("5. Large Array Stress Test");
    int N = 100000;
    vector<long long> big(N);
    iota(big.begin(), big.end(), 1LL);  // 1..N
    SegmentTree<long long> bigTree(big, [](long long a, long long b){ return a+b; }, 0LL);

    long long expected = (long long)N*(N+1)/2;
    long long got      = bigTree.query(0, N-1);
    cout << "Sum(1.." << N << ") expected=" << expected << " got=" << got << " -> " << (expected==got?"PASS":"FAIL") << "\n";

    bigTree.updatePoint(0, 0);  // replace 1 with 0
    cout << "After set[0]=0: sum=" << bigTree.query(0, N-1) << " (expected " << expected-1 << ") -> " << (bigTree.query(0,N-1)==expected-1?"PASS":"FAIL") << "\n";

    bigTree.addRange(0, N-1, 1);  // add 1 to every element
    cout << "After addRange(0," << N-1 << ",+1): sum=" << bigTree.query(0,N-1)
         << " (expected " << expected-1+N << ") -> "
         << (bigTree.query(0,N-1)==expected-1+N?"PASS":"FAIL") << "\n";

    sep("6. Error Handling");
    try { sumTree.query(3, 1); } catch(const out_of_range& e) { cout << "Caught: " << e.what() << "\n"; }
    try { sumTree.updatePoint(999, 1); } catch(const out_of_range& e) { cout << "Caught: " << e.what() << "\n"; }

    return 0;
}