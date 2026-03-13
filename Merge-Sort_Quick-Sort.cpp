#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <iomanip>
using namespace std;

// -- Utilities --------------------------------------------------------------
void printArray(const vector<int>& arr, const string& label = "") {
    if (!label.empty()) cout << label << ": ";
    cout << "[ ";
    for (size_t i = 0; i < arr.size(); i++) {
        cout << arr[i];
        if (i + 1 < arr.size()) cout << ", ";
    }
    cout << " ]\n";
}

vector<int> randomArray(int size, int minVal = 1, int maxVal = 999) {
    vector<int> arr(size);
    for (int& x : arr) x = minVal + rand() % (maxVal - minVal + 1);
    return arr;
}

// -- Merge Sort -------------------------------------------------------------
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(arr.begin() + left,  arr.begin() + mid + 1);
    vector<int> R(arr.begin() + mid + 1, arr.begin() + right + 1);

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else               arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void mergeSortHelper(vector<int>& arr, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSortHelper(arr, left, mid);
    mergeSortHelper(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

void mergeSort(vector<int>& arr) {
    if (arr.size() > 1)
        mergeSortHelper(arr, 0, (int)arr.size() - 1);
}

// -- Quick Sort -------------------------------------------------------------
// Median-of-three pivot to avoid worst-case on sorted input
int medianPivot(vector<int>& arr, int low, int high) {
    int mid = low + (high - low) / 2;
    if (arr[low] > arr[mid]) swap(arr[low], arr[mid]);
    if (arr[low] > arr[high]) swap(arr[low], arr[high]);
    if (arr[mid] > arr[high]) swap(arr[mid], arr[high]);
    // Place pivot at high-1
    swap(arr[mid], arr[high]);
    return arr[high];
}

int partition(vector<int>& arr, int low, int high) {
    int pivot = medianPivot(arr, low, high);
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) swap(arr[++i], arr[j]);
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSortHelper(vector<int>& arr, int low, int high) {
    if (low >= high) return;
    int pi = partition(arr, low, high);
    quickSortHelper(arr, low,    pi - 1);
    quickSortHelper(arr, pi + 1, high);
}

void quickSort(vector<int>& arr) {
    if (arr.size() > 1)
        quickSortHelper(arr, 0, (int)arr.size() - 1);
}

// -- Insertion Sort (for comparison) ---------------------------------------
void insertionSort(vector<int>& arr) {
    for (size_t i = 1; i < arr.size(); i++) {
        int key = arr[i];
        int j   = (int)i - 1;
        while (j >= 0 && arr[j] > key) { arr[j + 1] = arr[j]; j--; }
        arr[j + 1] = key;
    }
}

// -- Benchmark -------------------------------------------------------------
using SortFn = void(*)(vector<int>&);

double benchmark(SortFn fn, vector<int> arr) {
    auto start = chrono::high_resolution_clock::now();
    fn(arr);
    auto end   = chrono::high_resolution_clock::now();
    return chrono::duration<double, milli>(end - start).count();
}

int main() {
    srand(42);

    cout << "=== Sorting Algorithms: Merge Sort & Quick Sort ===\n\n";

    // -- Demo on small array --
    vector<int> demo = {38, 27, 43, 3, 9, 82, 10, 1, 65, 17};
    printArray(demo, "Original ");

    vector<int> ms = demo;
    mergeSort(ms);
    printArray(ms, "Merge Sort");

    vector<int> qs = demo;
    quickSort(qs);
    printArray(qs, "Quick Sort");

    // -- Verify both match std::sort --
    vector<int> ref = demo;
    sort(ref.begin(), ref.end());
    cout << "\nBoth match std::sort: "
         << ((ms == ref && qs == ref) ? "YES [OK]" : "NO [FAIL]") << "\n";

    // -- Edge cases --
    cout << "\n-- Edge Cases --\n";

    vector<int> empty = {};
    mergeSort(empty);
    quickSort(empty);
    cout << "Empty array      : OK\n";

    vector<int> single = {42};
    mergeSort(single);
    quickSort(single);
    cout << "Single element   : " << single[0] << " OK\n";

    vector<int> allSame = {5, 5, 5, 5, 5};
    mergeSort(allSame);
    printArray(allSame, "All same (merge)");

    vector<int> sorted = {1, 2, 3, 4, 5, 6, 7};
    quickSort(sorted);
    printArray(sorted, "Already sorted  ");

    vector<int> rev = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    mergeSort(rev);
    printArray(rev, "Reverse (merge) ");

    // -- Benchmark --
    cout << "\n-- Performance Benchmark --\n";
    cout << left
         << setw(18) << "Algorithm"
         << setw(14) << "n=1,000"
         << setw(14) << "n=10,000"
         << setw(14) << "n=100,000"
         << "\n";
    cout << string(60, '-') << "\n";

    struct Algo { string name; SortFn fn; };
    vector<Algo> algos = {
        { "Merge Sort",     mergeSort     },
        { "Quick Sort",     quickSort     },
        { "Insertion Sort", insertionSort },
    };

    for (auto& algo : algos) {
        cout << setw(18) << algo.name;
        for (int n : {1000, 10000, 100000}) {
            if (n == 100000 && algo.name == "Insertion Sort") {
                cout << setw(14) << "skip (slow)";
                continue;
            }
            vector<int> arr = randomArray(n);
            double ms_time = benchmark(algo.fn, arr);
            cout << setw(14) << (to_string((int)ms_time) + " ms");
        }
        cout << "\n";
    }

    cout << "\n-- Complexity Summary --\n";
    cout << left << setw(18) << "Algorithm"
                 << setw(16) << "Best"
                 << setw(16) << "Average"
                 << setw(16) << "Worst"
                 << setw(10) << "Space\n";
    cout << string(76, '-') << "\n";

    vector<vector<string>> table = {
        { "Merge Sort",     "O(n log n)", "O(n log n)", "O(n log n)", "O(n)"  },
        { "Quick Sort",     "O(n log n)", "O(n log n)", "O(n^2)",     "O(log n)" },
        { "Insertion Sort", "O(n)",       "O(n^2)",     "O(n^2)",     "O(1)"  },
    };

    for (auto& row : table) {
        for (size_t i = 0; i < row.size(); i++)
            cout << setw(i == 0 ? 18 : 16) << row[i];
        cout << "\n";
    }

    return 0;
}