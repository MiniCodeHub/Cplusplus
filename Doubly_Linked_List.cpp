#include <iostream>
#include <stdexcept>
using namespace std;

struct Node {
    int data;
    Node* prev;
    Node* next;
    Node(int val) : data(val), prev(nullptr), next(nullptr) {}
};

class DoublyLinkedList {
private:
    Node* head;
    Node* tail;
    int size;

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), size(0) {}

    // Insert at front
    void insertFront(int val) {
        Node* n = new Node(val);
        if (!head) { head = tail = n; }
        else {
            n->next    = head;
            head->prev = n;
            head       = n;
        }
        size++;
    }

    // Insert at back
    void insertBack(int val) {
        Node* n = new Node(val);
        if (!tail) { head = tail = n; }
        else {
            n->prev    = tail;
            tail->next = n;
            tail       = n;
        }
        size++;
    }

    // Insert at position (1-indexed)
    void insertAt(int val, int pos) {
        if (pos <= 1)    { insertFront(val); return; }
        if (pos > size)  { insertBack(val);  return; }
        Node* n    = new Node(val);
        Node* curr = head;
        for (int i = 1; i < pos - 1; i++) curr = curr->next;
        n->next       = curr->next;
        n->prev       = curr;
        if (curr->next) curr->next->prev = n;
        curr->next    = n;
        if (!n->next) tail = n;
        size++;
    }

    // Delete by value
    bool deleteByValue(int val) {
        Node* curr = head;
        while (curr && curr->data != val) curr = curr->next;
        if (!curr) { cout << val << " not found.\n"; return false; }
        if (curr->prev) curr->prev->next = curr->next;
        else            head = curr->next;
        if (curr->next) curr->next->prev = curr->prev;
        else            tail = curr->prev;
        delete curr;
        size--;
        cout << val << " deleted.\n";
        return true;
    }

    // Delete at position (1-indexed)
    bool deleteAt(int pos) {
        if (isEmpty() || pos < 1 || pos > size) {
            cout << "Invalid position.\n"; return false;
        }
        Node* curr = head;
        for (int i = 1; i < pos; i++) curr = curr->next;
        cout << curr->data << " deleted (pos " << pos << ").\n";
        if (curr->prev) curr->prev->next = curr->next;
        else            head = curr->next;
        if (curr->next) curr->next->prev = curr->prev;
        else            tail = curr->prev;
        delete curr;
        size--;
        return true;
    }

    // Search - returns 1-indexed position or -1
    int search(int val) const {
        Node* curr = head;
        int pos = 1;
        while (curr) {
            if (curr->data == val) return pos;
            curr = curr->next;
            pos++;
        }
        return -1;
    }

    // Traverse forward
    void displayForward() const {
        if (isEmpty()) { cout << "List is empty.\n"; return; }
        cout << "Forward : NULL <-> ";
        Node* curr = head;
        while (curr) {
            cout << curr->data;
            if (curr->next) cout << " <-> ";
            curr = curr->next;
        }
        cout << " <-> NULL\n";
    }

    // Traverse backward
    void displayBackward() const {
        if (isEmpty()) { cout << "List is empty.\n"; return; }
        cout << "Backward: NULL <-> ";
        Node* curr = tail;
        while (curr) {
            cout << curr->data;
            if (curr->prev) cout << " <-> ";
            curr = curr->prev;
        }
        cout << " <-> NULL\n";
    }

    // Reverse the list in-place
    void reverse() {
        Node* curr = head;
        Node* temp = nullptr;
        while (curr) {
            temp       = curr->prev;
            curr->prev = curr->next;
            curr->next = temp;
            curr       = curr->prev;    // move to next (was prev)
        }
        if (temp) head = temp->prev;    // temp is last processed node
        swap(head, tail);
        cout << "List reversed.\n";
    }

    bool isEmpty() const { return head == nullptr; }
    int  getSize() const { return size; }

    int front() const {
        if (isEmpty()) throw runtime_error("List is empty.");
        return head->data;
    }
    int back() const {
        if (isEmpty()) throw runtime_error("List is empty.");
        return tail->data;
    }

    ~DoublyLinkedList() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

int main() {
    cout << "=== Doubly Linked List Demo ===\n\n";

    DoublyLinkedList list;

    cout << "-- Insert at back: 10, 20, 30, 40 --\n";
    list.insertBack(10);
    list.insertBack(20);
    list.insertBack(30);
    list.insertBack(40);
    list.displayForward();

    cout << "\n-- Insert 5 at front --\n";
    list.insertFront(5);
    list.displayForward();

    cout << "\n-- Insert 25 at position 4 --\n";
    list.insertAt(25, 4);
    list.displayForward();

    cout << "\n-- Backward traversal --\n";
    list.displayBackward();

    cout << "\n-- Search for 25 and 99 --\n";
    int pos = list.search(25);
    cout << "25 found at position: " << (pos != -1 ? to_string(pos) : "not found") << "\n";
    pos = list.search(99);
    cout << "99 found at position: " << (pos != -1 ? to_string(pos) : "not found") << "\n";

    cout << "\n-- Delete by value: 20 --\n";
    list.deleteByValue(20);
    list.displayForward();

    cout << "\n-- Delete at position 2 --\n";
    list.deleteAt(2);
    list.displayForward();

    cout << "\n-- Head: " << list.front() << "  Tail: " << list.back() << " --\n";
    cout << "Size: " << list.getSize() << "\n";

    cout << "\n-- Reverse the list --\n";
    list.reverse();
    list.displayForward();
    list.displayBackward();

    cout << "\n-- Deleting non-existent value 99 --\n";
    list.deleteByValue(99);

    return 0;
}