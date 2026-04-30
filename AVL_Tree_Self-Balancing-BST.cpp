#include <iostream>
using namespace std;

struct Node {
    int data;
    Node* left;
    Node* right;
    int height;

    Node(int value) {
        data = value;
        left = right = nullptr;
        height = 1;
    }
};

class AVLTree {
private:
    int getHeight(Node* node) {
        if (node == nullptr)
            return 0;

        return node->height;
    }

    int getBalance(Node* node) {
        if (node == nullptr)
            return 0;

        return getHeight(node->left) - getHeight(node->right);
    }

    int max(int a, int b) {
        return (a > b) ? a : b;
    }

    Node* rightRotate(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

        return x;
    }

    Node* leftRotate(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

        return y;
    }

    Node* insert(Node* node, int value) {
        if (node == nullptr)
            return new Node(value);

        if (value < node->data)
            node->left = insert(node->left, value);
        else if (value > node->data)
            node->right = insert(node->right, value);
        else
            return node;

        node->height = 1 + max(getHeight(node->left), getHeight(node->right));

        int balance = getBalance(node);

        // Left Left
        if (balance > 1 && value < node->left->data)
            return rightRotate(node);

        // Right Right
        if (balance < -1 && value > node->right->data)
            return leftRotate(node);

        // Left Right
        if (balance > 1 && value > node->left->data) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Left
        if (balance < -1 && value < node->right->data) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    void inorder(Node* root) {
        if (root != nullptr) {
            inorder(root->left);
            cout << root->data << " ";
            inorder(root->right);
        }
    }

public:
    Node* root = nullptr;

    void insertValue(int value) {
        root = insert(root, value);
    }

    void display() {
        cout << "Inorder Traversal: ";
        inorder(root);
        cout << endl;
    }
};

int main() {
    AVLTree tree;

    tree.insertValue(10);
    tree.insertValue(20);
    tree.insertValue(30);
    tree.insertValue(40);
    tree.insertValue(50);
    tree.insertValue(25);

    tree.display();

    return 0;
}