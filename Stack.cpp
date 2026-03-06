#include <iostream>
#include <stdexcept>
#include <vector>
using namespace std;

struct Node {
    int data;
    Node* next;
    Node(int val) : data(val), next(nullptr) {}
};

class Stack {
private:
    Node* top;
    int size;

public:
    Stack() : top(nullptr), size(0) {}

    // Push element onto stack
    void push(int val) {
        Node* newNode = new Node(val);
        newNode->next = top;
        top = newNode;
        size++;
        cout << val << " pushed. (size: " << size << ")\n";
    }

    // Pop element from stack
    int pop() {
        if (isEmpty())
            throw underflow_error("Stack underflow: cannot pop from empty stack.");
        Node* temp = top;
        int val = temp->data;
        top = top->next;
        delete temp;
        size--;
        return val;
    }

    // Peek at top element
    int peek() const {
        if (isEmpty())
            throw underflow_error("Stack is empty: nothing to peek.");
        return top->data;
    }

    bool isEmpty() const {
        return top == nullptr;
    }

    int getSize() const {
        return size;
    }

    // Display all elements (top -> bottom)
    void display() const {
        if (isEmpty()) {
            cout << "Stack is empty.\n";
            return;
        }
        cout << "Stack (top -> bottom): ";
        Node* temp = top;
        while (temp) {
            cout << temp->data;
            if (temp->next) cout << " -> ";
            temp = temp->next;
        }
        cout << "\n";
    }

    // Clear the entire stack
    void clear() {
        while (!isEmpty()) pop();
        cout << "Stack cleared.\n";
    }

    ~Stack() {
        while (!isEmpty()) {
            Node* temp = top;
            top = top->next;
            delete temp;
        }
    }
};

// -- Application: Balanced Parentheses Checker --
bool isBalanced(const string& expr) {
    Stack s;
    for (char ch : expr) {
        if (ch == '(' || ch == '[' || ch == '{') {
            s.push(ch);
        } else if (ch == ')' || ch == ']' || ch == '}') {
            if (s.isEmpty()) return false;
            char top = s.pop();
            if ((ch == ')' && top != '(') ||
                (ch == ']' && top != '[') ||
                (ch == '}' && top != '{'))
                return false;
        }
    }
    return s.isEmpty();
}

// -- Application: Reverse a string using stack --
string reverseString(const string& str) {
    Stack s;
    for (char ch : str) s.push((int)ch);
    string result = "";
    while (!s.isEmpty()) result += (char)s.pop();
    return result;
}

// -- Application: Decimal to Binary --
string decToBinary(int n) {
    if (n == 0) return "0";
    Stack s;
    while (n > 0) {
        s.push(n % 2);
        n /= 2;
    }
    string result = "";
    while (!s.isEmpty()) result += to_string(s.pop());
    return result;
}

int main() {
    cout << "=== Stack Using Linked List ===\n\n";

    Stack stack;

    cout << "-- Pushing elements: 10, 20, 30, 40, 50 --\n";
    stack.push(10);
    stack.push(20);
    stack.push(30);
    stack.push(40);
    stack.push(50);

    cout << "\n";
    stack.display();
    cout << "Size: " << stack.getSize() << "\n";
    cout << "Peek (top): " << stack.peek() << "\n";

    cout << "\n-- Popping 2 elements --\n";
    cout << "Popped: " << stack.pop() << "\n";
    cout << "Popped: " << stack.pop() << "\n";

    cout << "\n";
    stack.display();

    cout << "\n-- Testing exception on empty stack --\n";
    Stack emptyStack;
    try {
        emptyStack.pop();
    } catch (const underflow_error& e) {
        cout << "Caught exception: " << e.what() << "\n";
    }

    cout << "\n-- Clearing the stack --\n";
    stack.clear();
    stack.display();

    cout << "\n=== Stack Applications ===\n\n";

    cout << "-- Balanced Parentheses Checker --\n";
    vector<string> exprs = {
        "{[()]}",
        "((()])",
        "[{()}]",
        "(((",
        "{[}"
    };
    for (const auto& e : exprs)
        cout << "\"" << e << "\" -> " << (isBalanced(e) ? "Balanced [OK]" : "Not balanced [X]") << "\n";

    cout << "\n-- String Reversal --\n";
    string original = "Hello, Stack!";
    cout << "Original : " << original << "\n";
    cout << "Reversed : " << reverseString(original) << "\n";

    cout << "\n-- Decimal to Binary --\n";
    for (int n : {0, 1, 5, 10, 42, 255})
        cout << n << " in binary = " << decToBinary(n) << "\n";

    return 0;
}