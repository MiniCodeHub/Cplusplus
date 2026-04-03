#include <iostream>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <iomanip>
#include <string>
#include <functional>
using namespace std;

// -- Generic Min Heap -------------------------------------------------------
template<typename T, typename Compare = less<T>>
class MinHeap {
private:
    vector<T>  data;
    Compare    cmp;   // cmp(a,b) = true means a has higher priority (comes first)

    // Index helpers
    int parent(int i) const { return (i - 1) / 2; }
    int left(int i)   const { return 2 * i + 1; }
    int right(int i)  const { return 2 * i + 2; }

    void heapifyUp(int i) {
        while (i > 0 && cmp(data[i], data[parent(i)])) {
            swap(data[i], data[parent(i)]);
            i = parent(i);
        }
    }

    void heapifyDown(int i) {
        int n = (int)data.size();
        while (true) {
            int smallest = i;
            int l = left(i), r = right(i);
            if (l < n && cmp(data[l], data[smallest])) smallest = l;
            if (r < n && cmp(data[r], data[smallest])) smallest = r;
            if (smallest == i) break;
            swap(data[i], data[smallest]);
            i = smallest;
        }
    }

public:
    MinHeap() = default;

    // Build from a vector in O(n)
    explicit MinHeap(vector<T> arr, Compare c = Compare()) : data(move(arr)), cmp(c) {
        for (int i = (int)data.size() / 2 - 1; i >= 0; i--)
            heapifyDown(i);
    }

    void push(const T& val) {
        data.push_back(val);
        heapifyUp((int)data.size() - 1);
    }

    T top() const {
        if (empty()) throw underflow_error("Heap is empty.");
        return data[0];
    }

    void pop() {
        if (empty()) throw underflow_error("Heap is empty.");
        data[0] = data.back();
        data.pop_back();
        if (!empty()) heapifyDown(0);
    }

    T extractMin() {
        T val = top();
        pop();
        return val;
    }

    // Decrease key at index i to newVal (for Dijkstra etc.)
    void decreaseKey(int i, const T& newVal) {
        if (i < 0 || i >= (int)data.size()) throw out_of_range("Index out of range.");
        if (cmp(data[i], newVal)) throw invalid_argument("New value is not smaller than current.");
        data[i] = newVal;
        heapifyUp(i);
    }

    // Delete element at index i
    void deleteAt(int i) {
        if (i < 0 || i >= (int)data.size()) throw out_of_range("Index out of range.");
        // Move to top by giving it the highest priority (smallest possible)
        // We'll just swap with last, remove, and re-heapify
        swap(data[i], data.back());
        data.pop_back();
        if (i < (int)data.size()) {
            heapifyDown(i);
            heapifyUp(i);
        }
    }

    bool   empty() const  { return data.empty(); }
    int    size()  const  { return (int)data.size(); }
    const  vector<T>& raw() const { return data; }

    // Heapsort: extract all elements in sorted order
    vector<T> heapSort() {
        vector<T> sorted;
        MinHeap<T,Compare> copy(data, cmp);
        while (!copy.empty()) sorted.push_back(copy.extractMin());
        return sorted;
    }

    // Validate heap property
    bool isValid() const {
        for (int i = 1; i < (int)data.size(); i++) {
            if (cmp(data[i], data[parent(i)])) return false;
        }
        return true;
    }

    // Print as tree (up to 4 levels)
    void printTree() const {
        if (empty()) { cout << "(empty)\n"; return; }
        int n = (int)data.size();
        int level = 0, count = 1, idx = 0;
        while (idx < n && level < 4) {
            cout << string(max(0, 16 - level * 4), ' ');
            for (int i = 0; i < count && idx < n; i++, idx++) {
                cout << setw(4) << data[idx] << " ";
            }
            cout << "\n";
            count *= 2;
            level++;
        }
        if (idx < n) cout << "  ... (" << n - idx << " more nodes)\n";
    }

    // Print internal array
    void printArray(const string& label = "Heap") const {
        cout << label << " [" << size() << "]: [ ";
        for (const auto& v : data) cout << v << " ";
        cout << "]\n";
    }
};

// -- Application: K Smallest Elements --------------------------------------
vector<int> kSmallest(vector<int> arr, int k) {
    MinHeap<int> h(arr);
    vector<int> result;
    for (int i = 0; i < k && !h.empty(); i++)
        result.push_back(h.extractMin());
    return result;
}

// -- Application: Merge K sorted arrays ------------------------------------
vector<int> mergeKSorted(const vector<vector<int>>& arrays) {
    struct Entry { int val, arr, idx;
        bool operator>(const Entry& o) const { return val > o.val; }
    };
    // Max heap acting as min by using greater<>
    MinHeap<Entry, greater<Entry>> h;
    for (int i = 0; i < (int)arrays.size(); i++)
        if (!arrays[i].empty()) h.push({arrays[i][0], i, 0});

    vector<int> result;
    while (!h.empty()) {
        auto [val, arr, idx] = h.extractMin();
        result.push_back(val);
        if (idx + 1 < (int)arrays[arr].size())
            h.push({arrays[arr][idx+1], arr, idx+1});
    }
    return result;
}

// -- Application: Priority Queue simulation --------------------------------
struct Task {
    int    priority;
    string name;
    bool operator<(const Task& o) const { return priority < o.priority; }
};

void sep(const string& t) {
    cout << "\n" << string(54, '=') << "\n " << t << "\n" << string(54, '=') << "\n";
}

void printVec(const vector<int>& v, const string& label = "") {
    if (!label.empty()) cout << label << ": ";
    cout << "[ ";
    for (int x : v) cout << x << " ";
    cout << "]\n";
}

int main() {
    cout << "=== Min Heap Implementation from Scratch ===\n";

    sep("1. Basic Operations");
    MinHeap<int> h;
    cout << "Pushing: 15, 10, 20, 5, 30, 1, 8\n";
    for (int v : {15, 10, 20, 5, 30, 1, 8}) h.push(v);
    h.printArray();
    h.printTree();
    cout << "Top (min): " << h.top() << "\n";
    cout << "Heap valid: " << (h.isValid() ? "YES" : "NO") << "\n";

    sep("2. Extract Min (heap sort order)");
    while (!h.empty())
        cout << h.extractMin() << " ";
    cout << "\n";

    sep("3. Build Heap from Array (O(n))");
    MinHeap<int> h2(vector<int>{38,27,43,3,9,82,10,1,65,17});
    h2.printArray("Built heap");
    h2.printTree();
    cout << "Heap valid: " << (h2.isValid() ? "YES" : "NO") << "\n";

    sep("4. Heap Sort");
    MinHeap<int> h3(vector<int>{64,34,25,12,22,11,90});
    auto sorted = h3.heapSort();
    printVec(sorted, "Sorted");

    sep("5. Decrease Key");
    MinHeap<int> h4(vector<int>{10, 20, 30, 40, 50});
    h4.printArray("Before");
    h4.decreaseKey(3, 5);  // change index 3 (40 -> 5)
    h4.printArray("After decreaseKey(3, 5)");
    cout << "New min: " << h4.top() << "\n";
    cout << "Heap valid: " << (h4.isValid() ? "YES" : "NO") << "\n";

    sep("6. Delete at Index");
    MinHeap<int> h5(vector<int>{1, 3, 5, 7, 9, 11});
    h5.printArray("Before");
    h5.deleteAt(2);  // delete index 2 (5)
    h5.printArray("After deleteAt(2)");
    cout << "Heap valid: " << (h5.isValid() ? "YES" : "NO") << "\n";

    sep("7. K Smallest Elements");
    vector<int> arr = {7, 10, 4, 3, 20, 15, 1, 8, 5};
    printVec(arr, "Input");
    for (int k : {1, 3, 5}) {
        auto res = kSmallest(arr, k);
        cout << k << " smallest: [ ";
        for (int x : res) cout << x << " ";
        cout << "]\n";
    }

    sep("8. Merge K Sorted Arrays");
    vector<vector<int>> arrs = {{1,4,7},{2,5,8},{3,6,9},{0,10,20}};
    cout << "Arrays: {1,4,7}, {2,5,8}, {3,6,9}, {0,10,20}\n";
    printVec(mergeKSorted(arrs), "Merged");

    sep("9. Task Priority Queue");
    MinHeap<Task> taskQ;
    taskQ.push({3, "Write tests"});
    taskQ.push({1, "Fix critical bug"});
    taskQ.push({2, "Code review"});
    taskQ.push({1, "Deploy hotfix"});
    taskQ.push({4, "Update docs"});
    cout << "Processing tasks by priority:\n";
    while (!taskQ.empty()) {
        auto t = taskQ.extractMin();
        cout << "  [P" << t.priority << "] " << t.name << "\n";
    }

    sep("10. Error Handling");
    MinHeap<int> empty;
    try { empty.top(); } catch(const underflow_error& e) { cout << "Caught: " << e.what() << "\n"; }
    try { empty.pop(); } catch(const underflow_error& e) { cout << "Caught: " << e.what() << "\n"; }

    sep("11. Max Heap using greater<int>");
    MinHeap<int, greater<int>> maxH;
    for (int v : {3, 1, 4, 1, 5, 9, 2, 6}) maxH.push(v);
    maxH.printArray("Max heap");
    cout << "Top (max): " << maxH.top() << "\n";
    cout << "Extract all (desc): ";
    while (!maxH.empty()) cout << maxH.extractMin() << " ";
    cout << "\n";

    return 0;
}