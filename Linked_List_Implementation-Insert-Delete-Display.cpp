#include <iostream>
using namespace std;

struct Node {
    int data;
    Node* next;
    Node(int val) : data(val), next(nullptr) {}
};

class LinkedList {
private:
    Node* head;

public:
    LinkedList() : head(nullptr) {}

    // Insert at end
    void insertEnd(int val) {
        Node* newNode = new Node(val);
        if (!head) {
            head = newNode;
            return;
        }
        Node* temp = head;
        while (temp->next)
            temp = temp->next;
        temp->next = newNode;
    }

    // Insert at beginning
    void insertBegin(int val) {
        Node* newNode = new Node(val);
        newNode->next = head;
        head = newNode;
    }

    // Insert at a given position (1-indexed)
    void insertAt(int val, int pos) {
        if (pos <= 1) {
            insertBegin(val);
            return;
        }
        Node* newNode = new Node(val);
        Node* temp = head;
        for (int i = 1; i < pos - 1 && temp; i++)
            temp = temp->next;
        if (!temp) {
            cout << "Position out of range. Inserting at end.\n";
            insertEnd(val);
            return;
        }
        newNode->next = temp->next;
        temp->next = newNode;
    }

    // Delete by value
    void deleteByValue(int val) {
        if (!head) {
            cout << "List is empty.\n";
            return;
        }
        if (head->data == val) {
            Node* temp = head;
            head = head->next;
            delete temp;
            cout << val << " deleted.\n";
            return;
        }
        Node* temp = head;
        while (temp->next && temp->next->data != val)
            temp = temp->next;
        if (!temp->next) {
            cout << val << " not found.\n";
            return;
        }
        Node* toDelete = temp->next;
        temp->next = toDelete->next;
        delete toDelete;
        cout << val << " deleted.\n";
    }

    // Delete at position (1-indexed)
    void deleteAt(int pos) {
        if (!head) {
            cout << "List is empty.\n";
            return;
        }
        if (pos == 1) {
            Node* temp = head;
            head = head->next;
            cout << temp->data << " deleted.\n";
            delete temp;
            return;
        }
        Node* temp = head;
        for (int i = 1; i < pos - 1 && temp->next; i++)
            temp = temp->next;
        if (!temp->next) {
            cout << "Position out of range.\n";
            return;
        }
        Node* toDelete = temp->next;
        temp->next = toDelete->next;
        cout << toDelete->data << " deleted.\n";
        delete toDelete;
    }

    // Display the list
    void display() {
        if (!head) {
            cout << "List is empty.\n";
            return;
        }
        Node* temp = head;
        cout << "List: ";
        while (temp) {
            cout << temp->data;
            if (temp->next) cout << " -> ";
            temp = temp->next;
        }
        cout << " -> NULL\n";
    }

    int length() {
        int count = 0;
        Node* temp = head;
        while (temp) { count++; temp = temp->next; }
        return count;
    }

    ~LinkedList() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

int main() {
    LinkedList list;

    cout << "=== Linked List Demo ===\n\n";

    cout << "-- Inserting at end: 10, 20, 30, 40 --\n";
    list.insertEnd(10);
    list.insertEnd(20);
    list.insertEnd(30);
    list.insertEnd(40);
    list.display();

    cout << "\n-- Inserting 5 at beginning --\n";
    list.insertBegin(5);
    list.display();

    cout << "\n-- Inserting 25 at position 4 --\n";
    list.insertAt(25, 4);
    list.display();

    cout << "\n-- Deleting by value: 20 --\n";
    list.deleteByValue(20);
    list.display();

    cout << "\n-- Deleting at position 2 --\n";
    list.deleteAt(2);
    list.display();

    cout << "\n-- Trying to delete non-existent value 99 --\n";
    list.deleteByValue(99);

    cout << "\nFinal list length: " << list.length() << "\n";
    list.display();

    return 0;
}