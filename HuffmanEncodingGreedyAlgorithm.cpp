#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>

using namespace std;

// Huffman Tree Node
struct Node {
    char ch;
    int freq;
    Node* left;
    Node* right;

    Node(char character, int frequency) {
        ch = character;
        freq = frequency;
        left = right = nullptr;
    }
};

// Compare function for Min Heap
struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

// Generate Huffman Codes
void generateCodes(
    Node* root,
    string code,
    unordered_map<char, string>& huffmanCodes
) {
    if (root == nullptr)
        return;

    // Leaf node
    if (!root->left && !root->right) {
        huffmanCodes[root->ch] = code;
    }

    generateCodes(root->left, code + "0", huffmanCodes);
    generateCodes(root->right, code + "1", huffmanCodes);
}

// Huffman Encoding
void huffmanEncoding(string text) {

    unordered_map<char, int> frequency;

    // Count frequency
    for (char ch : text) {
        frequency[ch]++;
    }

    // Min Heap
    priority_queue<Node*, vector<Node*>, Compare> minHeap;

    // Create nodes
    for (auto pair : frequency) {
        minHeap.push(new Node(pair.first, pair.second));
    }

    // Build Huffman Tree
    while (minHeap.size() > 1) {

        Node* left = minHeap.top();
        minHeap.pop();

        Node* right = minHeap.top();
        minHeap.pop();

        Node* merged = new Node('\0', left->freq + right->freq);

        merged->left = left;
        merged->right = right;

        minHeap.push(merged);
    }

    Node* root = minHeap.top();

    unordered_map<char, string> huffmanCodes;

    generateCodes(root, "", huffmanCodes);

    // Print codes
    cout << "Huffman Codes:\n";

    for (auto pair : huffmanCodes) {
        cout << pair.first << " : " << pair.second << endl;
    }

    // Encode text
    string encodedText = "";

    for (char ch : text) {
        encodedText += huffmanCodes[ch];
    }

    cout << "\nEncoded Text:\n";
    cout << encodedText << endl;
}

int main() {

    string text = "huffman coding";

    cout << "Original Text:\n";
    cout << text << endl << endl;

    huffmanEncoding(text);

    return 0;
}