#include <iostream>
#include <stdexcept>
#include <string>
using namespace std;

struct Node {
    int data;
    Node* next;
    Node(int val) : data(val), next(nullptr) {}
};

class Queue {
private:
    Node* front;
    Node* rear;
    int size;

public:
    Queue() : front(nullptr), rear(nullptr), size(0) {}

    // Enqueue - add to rear
    void enqueue(int val) {
        Node* newNode = new Node(val);
        if (!rear) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        size++;
        cout << val << " enqueued. (size: " << size << ")\n";
    }

    // Dequeue - remove from front
    int dequeue() {
        if (isEmpty())
            throw underflow_error("Queue underflow: cannot dequeue from empty queue.");
        Node* temp = front;
        int val = temp->data;
        front = front->next;
        if (!front) rear = nullptr;
        delete temp;
        size--;
        return val;
    }

    // Peek at front
    int peekFront() const {
        if (isEmpty())
            throw underflow_error("Queue is empty.");
        return front->data;
    }

    // Peek at rear
    int peekRear() const {
        if (isEmpty())
            throw underflow_error("Queue is empty.");
        return rear->data;
    }

    bool isEmpty() const { return front == nullptr; }
    int getSize() const  { return size; }

    // Display front -> rear
    void display() const {
        if (isEmpty()) { cout << "Queue is empty.\n"; return; }
        Node* temp = front;
        cout << "Queue (front -> rear): ";
        while (temp) {
            cout << temp->data;
            if (temp->next) cout << " -> ";
            temp = temp->next;
        }
        cout << "\n";
    }

    void clear() {
        while (!isEmpty()) dequeue();
        cout << "Queue cleared.\n";
    }

    ~Queue() { clear(); }
};

// -- Application: Print Queue Simulator ------------------------------------
class PrintQueue {
private:
    struct Job {
        int id;
        string name;
        int pages;
        Job* next;
        Job(int i, const string& n, int p) : id(i), name(n), pages(p), next(nullptr) {}
    };
    Job* front;
    Job* rear;
    int jobCount;

public:
    PrintQueue() : front(nullptr), rear(nullptr), jobCount(0) {}

    void addJob(const string& name, int pages) {
        Job* j = new Job(++jobCount, name, pages);
        if (!rear) { front = rear = j; }
        else { rear->next = j; rear = j; }
        cout << "[+] Job #" << j->id << " added: \"" << name << "\" (" << pages << " pages)\n";
    }

    void processNext() {
        if (!front) { cout << "No jobs in queue.\n"; return; }
        Job* j = front;
        cout << "[*] Printing Job #" << j->id << ": \"" << j->name << "\" (" << j->pages << " pages)... Done!\n";
        front = front->next;
        if (!front) rear = nullptr;
        delete j;
    }

    void showQueue() const {
        if (!front) { cout << "Print queue is empty.\n"; return; }
        cout << "Print Queue:\n";
        Job* temp = front;
        int pos = 1;
        while (temp) {
            cout << "  " << pos++ << ". Job #" << temp->id
                 << " - \"" << temp->name << "\" (" << temp->pages << " pages)\n";
            temp = temp->next;
        }
    }

    ~PrintQueue() {
        while (front) {
            Job* temp = front;
            front = front->next;
            delete temp;
        }
    }
};

int main() {
    cout << "=== Queue Using Linked List ===\n\n";

    Queue q;

    cout << "-- Enqueuing: 10, 20, 30, 40, 50 --\n";
    for (int v : {10, 20, 30, 40, 50}) q.enqueue(v);

    cout << "\n";
    q.display();
    cout << "Size      : " << q.getSize() << "\n";
    cout << "Front     : " << q.peekFront() << "\n";
    cout << "Rear      : " << q.peekRear()  << "\n";

    cout << "\n-- Dequeuing 2 elements --\n";
    cout << "Dequeued: " << q.dequeue() << "\n";
    cout << "Dequeued: " << q.dequeue() << "\n";
    q.display();

    cout << "\n-- Exception on empty queue --\n";
    Queue empty;
    try { empty.dequeue(); }
    catch (const underflow_error& e) { cout << "Caught: " << e.what() << "\n"; }

    cout << "\n-- Enqueue more and clear --\n";
    q.enqueue(60);
    q.enqueue(70);
    q.display();
    q.clear();
    q.display();

    cout << "\n=== Print Queue Simulator ===\n\n";
    PrintQueue pq;
    pq.addJob("Resume.pdf", 2);
    pq.addJob("Report.docx", 15);
    pq.addJob("Photo.png", 1);
    pq.addJob("Slides.pptx", 30);
    cout << "\n";
    pq.showQueue();
    cout << "\n-- Processing jobs one by one --\n";
    pq.processNext();
    pq.processNext();
    cout << "\n";
    pq.showQueue();
    cout << "\n-- Processing remaining jobs --\n";
    pq.processNext();
    pq.processNext();
    pq.processNext(); // should say queue empty
    return 0;
}