#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>
using namespace std;

struct Node {
    int data;
    Node* next;
    Node(int val) : data(val), next(nullptr) {}
};

class CircularLinkedList {
private:
    Node* tail; // tail->next == head
    int   sz;

public:
    CircularLinkedList() : tail(nullptr), sz(0) {}

    // Insert at front (head)
    void insertFront(int val) {
        Node* n = new Node(val);
        if (!tail) { n->next = n; tail = n; }
        else { n->next = tail->next; tail->next = n; }
        sz++;
    }

    // Insert at back (after tail)
    void insertBack(int val) {
        Node* n = new Node(val);
        if (!tail) { n->next = n; tail = n; }
        else { n->next = tail->next; tail->next = n; tail = n; }
        sz++;
    }

    // Insert at position (1-indexed)
    void insertAt(int val, int pos) {
        if (pos <= 1)  { insertFront(val); return; }
        if (pos > sz)  { insertBack(val);  return; }
        Node* curr = tail->next; // start at head
        for (int i = 1; i < pos - 1; i++) curr = curr->next;
        Node* n  = new Node(val);
        n->next  = curr->next;
        curr->next = n;
        sz++;
    }

    // Delete by value
    bool deleteByValue(int val) {
        if (!tail) { cout << "List is empty.\n"; return false; }
        Node* head = tail->next;
        // Special case: only one node
        if (head == tail && head->data == val) {
            delete head; tail = nullptr; sz--; cout << val << " deleted.\n"; return true;
        }
        Node* prev = tail;
        Node* curr = head;
        do {
            if (curr->data == val) {
                prev->next = curr->next;
                if (curr == tail) tail = prev;
                delete curr; sz--;
                cout << val << " deleted.\n"; return true;
            }
            prev = curr; curr = curr->next;
        } while (curr != head);
        cout << val << " not found.\n"; return false;
    }

    // Delete at position (1-indexed)
    bool deleteAt(int pos) {
        if (!tail || pos < 1 || pos > sz) { cout << "Invalid position.\n"; return false; }
        Node* head = tail->next;
        if (head == tail) {                // single node
            cout << head->data << " deleted.\n";
            delete head; tail = nullptr; sz--; return true;
        }
        Node* prev = tail;
        Node* curr = head;
        for (int i = 1; i < pos; i++) { prev = curr; curr = curr->next; }
        prev->next = curr->next;
        if (curr == tail) tail = prev;
        cout << curr->data << " deleted (pos " << pos << ").\n";
        delete curr; sz--; return true;
    }

    // Search - returns 1-indexed position or -1
    int search(int val) const {
        if (!tail) return -1;
        Node* curr = tail->next;
        int   pos  = 1;
        do {
            if (curr->data == val) return pos;
            curr = curr->next; pos++;
        } while (curr != tail->next);
        return -1;
    }

    // Display one full cycle
    void display() const {
        if (!tail) { cout << "List is empty.\n"; return; }
        cout << "CLL: ";
        Node* curr = tail->next;
        do {
            cout << curr->data;
            if (curr->next != tail->next) cout << " -> ";
            curr = curr->next;
        } while (curr != tail->next);
        cout << " -> (back to " << tail->next->data << ")\n";
    }

    // Traverse N steps from head (demonstrates circular property)
    void traverseSteps(int steps) const {
        if (!tail) { cout << "List is empty.\n"; return; }
        cout << "Traversing " << steps << " steps from head: ";
        Node* curr = tail->next;
        for (int i = 0; i < steps; i++) {
            cout << curr->data;
            if (i < steps - 1) cout << " -> ";
            curr = curr->next;
        }
        cout << "\n";
    }

    // Split into two halves
    pair<CircularLinkedList*, CircularLinkedList*> splitHalf() const {
        auto* a = new CircularLinkedList();
        auto* b = new CircularLinkedList();
        if (!tail) return {a, b};
        // Find midpoint with slow/fast pointers
        Node* slow = tail->next;
        Node* fast = tail->next;
        while (fast != tail && fast->next != tail) {
            slow = slow->next;
            fast = fast->next->next;
        }
        // First half: head -> slow
        Node* curr = tail->next;
        while (curr != slow->next) { a->insertBack(curr->data); curr = curr->next; }
        // Second half: slow->next -> tail
        while (curr != tail->next) { b->insertBack(curr->data); curr = curr->next; }
        return {a, b};
    }

    bool isEmpty() const { return tail == nullptr; }
    int  size()    const { return sz; }

    int front() const {
        if (!tail) throw runtime_error("List is empty.");
        return tail->next->data;
    }
    int back() const {
        if (!tail) throw runtime_error("List is empty.");
        return tail->data;
    }

    ~CircularLinkedList() {
        if (!tail) return;
        Node* curr = tail->next;
        tail->next = nullptr;
        while (curr) { Node* t = curr; curr = curr->next; delete t; }
    }
};

// -- Application: Josephus Problem -----------------------------------------
// n people in a circle, eliminate every k-th person; return survivor position
int josephus(int n, int k) {
    CircularLinkedList cll;
    for (int i = 1; i <= n; i++) cll.insertBack(i);
    cout << "Josephus (n=" << n << ", k=" << k << "): Elimination order: ";
    int eliminated = 0;
    int survivor = -1;
    while (cll.size() > 1) {
        // advance k-1 steps
        Node* tail_ptr = nullptr; // we need direct access - simulate with search
        // Use display-free traversal by re-building step logic
        // Build a temp array for this demo
        break; // handled below with array simulation
    }
    // Array simulation for Josephus
    vector<int> people(n);
    for (int i = 0; i < n; i++) people[i] = i + 1;
    int idx = 0;
    while (people.size() > 1) {
        idx = (idx + k - 1) % people.size();
        cout << people[idx];
        if (people.size() > 1) cout << " -> ";
        people.erase(people.begin() + idx);
        if (idx == (int)people.size()) idx = 0;
    }
    cout << "\nSurvivor: " << people[0] << "\n";
    return people[0];
}

int main() {
    cout << "=== Circular Linked List Demo ===\n\n";

    CircularLinkedList cll;

    cout << "-- Insert at back: 10, 20, 30, 40 --\n";
    cll.insertBack(10); cll.insertBack(20);
    cll.insertBack(30); cll.insertBack(40);
    cll.display();

    cout << "\n-- Insert 5 at front --\n";
    cll.insertFront(5);
    cll.display();

    cout << "\n-- Insert 25 at position 4 --\n";
    cll.insertAt(25, 4);
    cll.display();

    cout << "\nFront: " << cll.front() << "  Back: " << cll.back()
         << "  Size: " << cll.size() << "\n";

    cout << "\n-- Circular traversal: 10 steps --\n";
    cll.traverseSteps(10);

    cout << "\n-- Search for 25 and 99 --\n";
    int p = cll.search(25);
    cout << "25 -> position " << (p != -1 ? to_string(p) : "not found") << "\n";
    p = cll.search(99);
    cout << "99 -> " << (p != -1 ? to_string(p) : "not found") << "\n";

    cout << "\n-- Delete by value: 20 --\n";
    cll.deleteByValue(20);
    cll.display();

    cout << "\n-- Delete at position 1 (head) --\n";
    cll.deleteAt(1);
    cll.display();

    cout << "\n-- Delete at last position --\n";
    cll.deleteAt(cll.size());
    cll.display();

    cout << "\n-- Split into two halves --\n";
    auto [a, b] = cll.splitHalf();
    cout << "Half A -> "; a->display();
    cout << "Half B -> "; b->display();
    delete a; delete b;

    cout << "\n-- Delete non-existent value --\n";
    cll.deleteByValue(999);

    cout << "\n=== Josephus Problem ===\n\n";
    josephus(7, 3);
    cout << "\n";
    josephus(5, 2);

    return 0;
}