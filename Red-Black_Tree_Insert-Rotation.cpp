#include <iostream>
using namespace std;

enum Color {
    RED,
    BLACK
};

struct Node {
    int data;
    Color color;
    Node* left;
    Node* right;
    Node* parent;

    Node(int value) {
        data = value;
        color = RED;
        left = right = parent = nullptr;
    }
};

class RedBlackTree {
private:
    Node* root;

    void leftRotate(Node* x) {
        Node* y = x->right;

        x->right = y->left;

        if (y->left != nullptr)
            y->left->parent = x;

        y->parent = x->parent;

        if (x->parent == nullptr)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;

        y->left = x;
        x->parent = y;
    }

    void rightRotate(Node* y) {
        Node* x = y->left;

        y->left = x->right;

        if (x->right != nullptr)
            x->right->parent = y;

        x->parent = y->parent;

        if (y->parent == nullptr)
            root = x;
        else if (y == y->parent->left)
            y->parent->left = x;
        else
            y->parent->right = x;

        x->right = y;
        y->parent = x;
    }

    void fixInsert(Node* k) {
        while (k != root && k->parent->color == RED) {
            Node* parent = k->parent;
            Node* grandparent = parent->parent;

            // Parent is left child
            if (parent == grandparent->left) {
                Node* uncle = grandparent->right;

                // Case 1: Uncle is red
                if (uncle != nullptr && uncle->color == RED) {
                    grandparent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    k = grandparent;
                }
                else {
                    // Case 2: Left-Right
                    if (k == parent->right) {
                        k = parent;
                        leftRotate(k);
                    }

                    // Case 3: Left-Left
                    parent->color = BLACK;
                    grandparent->color = RED;
                    rightRotate(grandparent);
                }
            }
            else {
                Node* uncle = grandparent->left;

                // Mirror Case 1
                if (uncle != nullptr && uncle->color == RED) {
                    grandparent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    k = grandparent;
                }
                else {
                    // Mirror Case 2
                    if (k == parent->left) {
                        k = parent;
                        rightRotate(k);
                    }

                    // Mirror Case 3
                    parent->color = BLACK;
                    grandparent->color = RED;
                    leftRotate(grandparent);
                }
            }
        }

        root->color = BLACK;
    }

    void inorder(Node* node) {
        if (node == nullptr)
            return;

        inorder(node->left);

        cout << node->data
             << "("
             << (node->color == RED ? "R" : "B")
             << ") ";

        inorder(node->right);
    }

public:
    RedBlackTree() {
        root = nullptr;
    }

    void insert(int value) {
        Node* newNode = new Node(value);

        Node* parent = nullptr;
        Node* current = root;

        while (current != nullptr) {
            parent = current;

            if (newNode->data < current->data)
                current = current->left;
            else
                current = current->right;
        }

        newNode->parent = parent;

        if (parent == nullptr)
            root = newNode;
        else if (newNode->data < parent->data)
            parent->left = newNode;
        else
            parent->right = newNode;

        fixInsert(newNode);
    }

    void display() {
        cout << "Inorder Traversal:\n";
        inorder(root);
        cout << endl;
    }
};

int main() {
    RedBlackTree tree;

    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(15);
    tree.insert(25);
    tree.insert(5);

    tree.display();

    return 0;
}