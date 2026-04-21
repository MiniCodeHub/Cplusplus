#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

// -- Node-based Doubly-Linked Deque -----------------------------------------
template<typename T>
class Deque {
private:
    struct Node {
        T     data;
        Node* prev;
        Node* next;
        explicit Node(const T& val) : data(val), prev(nullptr), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int   sz;

public:
    Deque() : head(nullptr), tail(nullptr), sz(0) {}

    // -- Push --
    void pushFront(const T& val) {
        Node* n = new Node(val);
        if (!head) { head = tail = n; }
        else { n->next = head; head->prev = n; head = n; }
        sz++;
    }

    void pushBack(const T& val) {
        Node* n = new Node(val);
        if (!tail) { head = tail = n; }
        else { n->prev = tail; tail->next = n; tail = n; }
        sz++;
    }

    // -- Pop --
    T popFront() {
        if (empty()) throw underflow_error("Deque is empty: popFront");
        T val = head->data;
        Node* temp = head;
        head = head->next;
        if (head) head->prev = nullptr;
        else      tail = nullptr;
        delete temp;
        sz--;
        return val;
    }

    T popBack() {
        if (empty()) throw underflow_error("Deque is empty: popBack");
        T val = tail->data;
        Node* temp = tail;
        tail = tail->prev;
        if (tail) tail->next = nullptr;
        else      head = nullptr;
        delete temp;
        sz--;
        return val;
    }

    // -- Peek --
    T& front() {
        if (empty()) throw underflow_error("Deque is empty: front");
        return head->data;
    }
    T& back() {
        if (empty()) throw underflow_error("Deque is empty: back");
        return tail->data;
    }
    const T& front() const { return const_cast<Deque*>(this)->front(); }
    const T& back()  const { return const_cast<Deque*>(this)->back();  }

    // -- Index access (O(n)) --
    T& at(int i) {
        if (i < 0 || i >= sz) throw out_of_range("Index out of range.");
        Node* curr = head;
        for (int k = 0; k < i; k++) curr = curr->next;
        return curr->data;
    }

    // -- Utilities --
    bool empty() const { return sz == 0; }
    int  size()  const { return sz; }

    void clear() {
        while (!empty()) popFront();
    }

    // Rotate left by k positions
    void rotateLeft(int k) {
        if (sz <= 1) return;
        k %= sz;
        for (int i = 0; i < k; i++) pushBack(popFront());
    }

    // Rotate right by k positions
    void rotateRight(int k) {
        if (sz <= 1) return;
        k %= sz;
        for (int i = 0; i < k; i++) pushFront(popBack());
    }

    // Convert to vector
    vector<T> toVector() const {
        vector<T> v;
        Node* curr = head;
        while (curr) { v.push_back(curr->data); curr = curr->next; }
        return v;
    }

    void display(const string& label = "Deque") const {
        if (empty()) { cout << label << ": [empty]\n"; return; }
        cout << label << " [" << sz << "]: HEAD <-> ";
        Node* curr = head;
        while (curr) {
            cout << curr->data;
            if (curr->next) cout << " <-> ";
            curr = curr->next;
        }
        cout << " <-> TAIL\n";
    }

    ~Deque() { clear(); }
};

// -- Application 1: Sliding Window Maximum ---------------------------------
// Returns max of every window of size k
vector<int> slidingWindowMax(const vector<int>& arr, int k) {
    vector<int> result;
    Deque<int> dq;  // stores indices

    for (int i = 0; i < (int)arr.size(); i++) {
        // Remove out-of-window indices
        while (!dq.empty() && dq.front() < i - k + 1)
            dq.popFront();
        // Remove indices with smaller values (maintain decreasing order)
        while (!dq.empty() && arr[dq.back()] < arr[i])
            dq.popBack();
        dq.pushBack(i);
        if (i >= k - 1) result.push_back(arr[dq.front()]);
    }
    return result;
}

// -- Application 2: Palindrome Check using Deque ---------------------------
bool isPalindrome(const string& s) {
    Deque<char> dq;
    for (char c : s) if (isalpha(c)) dq.pushBack(tolower(c));
    while (dq.size() > 1) {
        if (dq.popFront() != dq.popBack()) return false;
    }
    return true;
}

// -- Application 3: Josephus Circle using Deque ----------------------------
int josephus(int n, int k) {
    Deque<int> circle;
    for (int i = 1; i <= n; i++) circle.pushBack(i);
    cout << "Josephus(n=" << n << ",k=" << k << ") elimination: ";
    while (circle.size() > 1) {
        circle.rotateLeft(k - 1);
        cout << circle.front() << " ";
        circle.popFront();
    }
    cout << "\nSurvivor: " << circle.front() << "\n";
    return circle.front();
}

void sep(const string& t) {
    cout << "\n" << string(52, '=') << "\n " << t << "\n" << string(52, '=') << "\n";
}

int main() {
    cout << "=== Deque (Double-Ended Queue) Implementation ===\n";

    Deque<int> dq;

    sep("1. Push operations");
    dq.pushBack(10);  dq.pushBack(20);  dq.pushBack(30);
    dq.pushFront(5);  dq.pushFront(1);
    dq.display();
    cout << "Front: " << dq.front() << "  Back: " << dq.back()
         << "  Size: " << dq.size() << "\n";

    sep("2. Pop operations");
    cout << "popFront: " << dq.popFront() << "\n";
    cout << "popBack:  " << dq.popBack()  << "\n";
    dq.display();

    sep("3. Index access");
    for (int i = 0; i < dq.size(); i++)
        cout << "at(" << i << ") = " << dq.at(i) << "\n";

    sep("4. Rotate");
    dq.display("Before rotate");
    dq.rotateLeft(2);
    dq.display("After rotateLeft(2)");
    dq.rotateRight(2);
    dq.display("After rotateRight(2)");

    sep("5. String Deque");
    Deque<string> words;
    words.pushBack("world"); words.pushFront("hello");
    words.pushBack("!"); words.pushFront("say:");
    words.display("Words");

    sep("6. Error handling");
    Deque<int> empty;
    try { empty.popFront(); } catch(const underflow_error& e) { cout << "Caught: " << e.what() << "\n"; }
    try { empty.at(5);      } catch(const out_of_range& e)    { cout << "Caught: " << e.what() << "\n"; }

    sep("7. App: Sliding Window Maximum");
    vector<int> arr = {1, 3, -1, -3, 5, 3, 6, 7};
    cout << "Array: [ "; for (int x : arr) cout << x << " "; cout << "]\n";
    for (int k : {3, 4}) {
        auto res = slidingWindowMax(arr, k);
        cout << "Window k=" << k << ": [ ";
        for (int x : res) cout << x << " ";
        cout << "]\n";
    }

    sep("8. App: Palindrome Check");
    vector<string> tests = {"racecar","hello","A man a plan a canal Panama","Never odd or even","abcba"};
    for (const auto& s : tests)
        cout << "\"" << s << "\": " << (isPalindrome(s) ? "PALINDROME" : "not palindrome") << "\n";

    sep("9. App: Josephus Problem");
    josephus(7, 3);
    cout << "\n";
    josephus(5, 2);

    return 0;
}