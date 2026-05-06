#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// Partition function
int partition(vector<int>& arr, int low, int high) {

    // Generate random pivot index
    int randomIndex = low + rand() % (high - low + 1);

    // Move random pivot to end
    swap(arr[randomIndex], arr[high]);

    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {

        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }

    swap(arr[i + 1], arr[high]);

    return i + 1;
}

// Quick Sort function
void quickSort(vector<int>& arr, int low, int high) {

    if (low < high) {

        int pivotIndex = partition(arr, low, high);

        quickSort(arr, low, pivotIndex - 1);
        quickSort(arr, pivotIndex + 1, high);
    }
}

int main() {

    srand(time(0));

    vector<int> arr = {9, 4, 7, 3, 10, 5, 1};

    cout << "Original Array:\n";

    for (int num : arr)
        cout << num << " ";

    quickSort(arr, 0, arr.size() - 1);

    cout << "\n\nSorted Array:\n";

    for (int num : arr)
        cout << num << " ";

    return 0;
}