#include <iostream>
#include <queue>
using namespace std;

struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

class BST {
private:
    Node* root;

    Node* insertHelper(Node* node, int val) {
        if (!node) return new Node(val);
        if (val < node->data)
            node->left = insertHelper(node->left, val);
        else if (val > node->data)
            node->right = insertHelper(node->right, val);
        else
            cout << val << " already exists in the BST.\n";
        return node;
    }

    void inorderHelper(Node* node) {
        if (!node) return;
        inorderHelper(node->left);
        cout << node->data << " ";
        inorderHelper(node->right);
    }

    void preorderHelper(Node* node) {
        if (!node) return;
        cout << node->data << " ";
        preorderHelper(node->left);
        preorderHelper(node->right);
    }

    void postorderHelper(Node* node) {
        if (!node) return;
        postorderHelper(node->left);
        postorderHelper(node->right);
        cout << node->data << " ";
    }

    bool searchHelper(Node* node, int val) {
        if (!node) return false;
        if (node->data == val) return true;
        if (val < node->data) return searchHelper(node->left, val);
        return searchHelper(node->right, val);
    }

    int heightHelper(Node* node) {
        if (!node) return 0;
        return 1 + max(heightHelper(node->left), heightHelper(node->right));
    }

    Node* findMin(Node* node) {
        while (node->left) node = node->left;
        return node;
    }

    Node* deleteHelper(Node* node, int val) {
        if (!node) {
            cout << val << " not found.\n";
            return node;
        }
        if (val < node->data)
            node->left = deleteHelper(node->left, val);
        else if (val > node->data)
            node->right = deleteHelper(node->right, val);
        else {
            // Node with only one child or no child
            if (!node->left) {
                Node* temp = node->right;
                delete node;
                return temp;
            } else if (!node->right) {
                Node* temp = node->left;
                delete node;
                return temp;
            }
            // Node with two children: get inorder successor
            Node* successor = findMin(node->right);
            node->data = successor->data;
            node->right = deleteHelper(node->right, successor->data);
        }
        return node;
    }

    void destroyTree(Node* node) {
        if (!node) return;
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }

    void printTree(Node* node, string prefix, bool isLeft) {
        if (!node) return;
        cout << prefix << (isLeft ? "|-- " : "\\-- ") << node->data << "\n";
        printTree(node->left,  prefix + (isLeft ? "|   " : "    "), true);
        printTree(node->right, prefix + (isLeft ? "|   " : "    "), false);
    }

public:
    BST() : root(nullptr) {}

    void insert(int val) {
        root = insertHelper(root, val);
    }

    void deleteNode(int val) {
        root = deleteHelper(root, val);
    }

    bool search(int val) {
        return searchHelper(root, val);
    }

    void inorder() {
        cout << "Inorder   (L -> Root -> R): ";
        inorderHelper(root);
        cout << "\n";
    }

    void preorder() {
        cout << "Preorder  (Root -> L -> R): ";
        preorderHelper(root);
        cout << "\n";
    }

    void postorder() {
        cout << "Postorder (L -> R -> Root): ";
        postorderHelper(root);
        cout << "\n";
    }

    void levelOrder() {
        if (!root) { cout << "Tree is empty.\n"; return; }
        cout << "Level Order (BFS): ";
        queue<Node*> q;
        q.push(root);
        while (!q.empty()) {
            Node* curr = q.front(); q.pop();
            cout << curr->data << " ";
            if (curr->left)  q.push(curr->left);
            if (curr->right) q.push(curr->right);
        }
        cout << "\n";
    }

    int height() {
        return heightHelper(root);
    }

    void printStructure() {
        if (!root) { cout << "(empty tree)\n"; return; }
        cout << root->data << "\n";
        printTree(root->left,  "", true);
        printTree(root->right, "", false);
    }

    ~BST() { destroyTree(root); }
};

int main() {
    BST tree;

    cout << "=== Binary Search Tree Demo ===\n\n";

    cout << "-- Inserting: 50, 30, 70, 20, 40, 60, 80, 10, 25 --\n";
    for (int val : {50, 30, 70, 20, 40, 60, 80, 10, 25})
        tree.insert(val);

    cout << "\n-- Tree Structure --\n";
    tree.printStructure();

    cout << "\n-- Traversals --\n";
    tree.inorder();
    tree.preorder();
    tree.postorder();
    tree.levelOrder();

    cout << "\n-- Tree Height: " << tree.height() << " --\n";

    cout << "\n-- Searching --\n";
    for (int val : {40, 99, 10}) {
        cout << "Search " << val << ": " << (tree.search(val) ? "Found" : "Not found") << "\n";
    }

    cout << "\n-- Deleting node with one child: 20 --\n";
    tree.deleteNode(20);
    tree.inorder();

    cout << "\n-- Deleting node with two children: 30 --\n";
    tree.deleteNode(30);
    tree.inorder();

    cout << "\n-- Deleting root: 50 --\n";
    tree.deleteNode(50);
    tree.inorder();

    cout << "\n-- Final Tree Structure --\n";
    tree.printStructure();

    return 0;
}