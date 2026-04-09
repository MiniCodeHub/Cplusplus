#include <iostream>
#include <string>
#include <unordered_map>
#include <queue>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <bitset>
#include <stdexcept>
using namespace std;

// -- Huffman Tree Node ------------------------------------------------------
struct HNode {
    char   ch;
    int    freq;
    HNode* left;
    HNode* right;

    HNode(char c, int f, HNode* l = nullptr, HNode* r = nullptr)
        : ch(c), freq(f), left(l), right(r) {}

    bool isLeaf() const { return !left && !right; }
};

// Min-heap comparator
struct HNodeCmp {
    bool operator()(HNode* a, HNode* b) const { return a->freq > b->freq; }
};

// -- Huffman Encoder --------------------------------------------------------
class HuffmanEncoder {
private:
    HNode* root;
    unordered_map<char,string> codes;   // char -> binary string
    unordered_map<string,char> decode;  // binary string -> char

    void buildCodes(HNode* node, const string& prefix) {
        if (!node) return;
        if (node->isLeaf()) {
            codes[node->ch]  = prefix.empty() ? "0" : prefix;
            decode[prefix.empty() ? "0" : prefix] = node->ch;
            return;
        }
        buildCodes(node->left,  prefix + "0");
        buildCodes(node->right, prefix + "1");
    }

    void destroyTree(HNode* node) {
        if (!node) return;
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }

    // Print tree structure
    void printTreeHelper(HNode* node, const string& prefix, bool isLeft) const {
        if (!node) return;
        cout << prefix << (isLeft ? "|-- " : "`-- ");
        if (node->isLeaf())
            cout << "'" << (node->ch == '\n' ? '\\' : node->ch) << "' (" << node->freq << ")\n";
        else
            cout << "[" << node->freq << "]\n";
        string child = prefix + (isLeft ? "|   " : "    ");
        printTreeHelper(node->left,  child, true);
        printTreeHelper(node->right, child, false);
    }

public:
    HuffmanEncoder() : root(nullptr) {}

    // Build the tree from a frequency map
    void build(const unordered_map<char,int>& freq) {
        destroyTree(root); root = nullptr;
        codes.clear(); decode.clear();

        priority_queue<HNode*, vector<HNode*>, HNodeCmp> pq;
        for (auto& [ch, f] : freq) pq.push(new HNode(ch, f));

        if (pq.empty()) return;

        // Edge case: single unique character
        if (pq.size() == 1) {
            HNode* only = pq.top(); pq.pop();
            root = new HNode('\0', only->freq, only, nullptr);
        } else {
            while (pq.size() > 1) {
                HNode* l = pq.top(); pq.pop();
                HNode* r = pq.top(); pq.pop();
                pq.push(new HNode('\0', l->freq + r->freq, l, r));
            }
            root = pq.top();
        }
        buildCodes(root, "");
    }

    // Build directly from a string
    void buildFromText(const string& text) {
        unordered_map<char,int> freq;
        for (char c : text) freq[c]++;
        build(freq);
    }

    // Encode a string to binary string
    string encode(const string& text) const {
        string bits;
        for (char c : text) {
            auto it = codes.find(c);
            if (it == codes.end()) throw runtime_error("Character not in codebook.");
            bits += it->second;
        }
        return bits;
    }

    // Decode a binary string back to text
    string decodeStr(const string& bits) const {
        if (!root) return "";
        string result;
        HNode* curr = root;
        for (char b : bits) {
            curr = (b == '0') ? curr->left : curr->right;
            if (!curr) throw runtime_error("Invalid encoded string.");
            if (curr->isLeaf()) {
                result += curr->ch;
                curr = root;
            }
        }
        return result;
    }

    const unordered_map<char,string>& getCodes() const { return codes; }

    // Stats
    void printCodeTable(const unordered_map<char,int>& freq) const {
        // Sort by freq descending
        vector<pair<char,int>> sorted(freq.begin(), freq.end());
        sort(sorted.begin(), sorted.end(), [](auto& a, auto& b){ return a.second > b.second; });

        cout << left
             << setw(8)  << "Char"
             << setw(8)  << "Freq"
             << setw(14) << "Code"
             << setw(6)  << "Bits"
             << setw(10) << "Contribution\n";
        cout << string(46, '-') << "\n";

        int totalBits = 0;
        for (auto& [ch, f] : sorted) {
            auto it = codes.find(ch);
            string code = (it != codes.end()) ? it->second : "?";
            int contribution = f * (int)code.size();
            totalBits += contribution;
            string chStr;
            if (ch == ' ')  chStr = "' '";
            else if (ch == '\n') chStr = "'\\n'";
            else chStr = string(1, '\'') + ch + '\'';
            cout << setw(8)  << chStr
                 << setw(8)  << f
                 << setw(14) << code
                 << setw(6)  << code.size()
                 << setw(10) << contribution << "\n";
        }
        cout << string(46, '-') << "\n";
        cout << "Total encoded bits: " << totalBits << "\n";
    }

    void printTree() const {
        if (!root) { cout << "(empty)\n"; return; }
        cout << "[" << root->freq << "]\n";
        printTreeHelper(root->left,  "", true);
        printTreeHelper(root->right, "", false);
    }

    ~HuffmanEncoder() { destroyTree(root); }
};

// -- Analysis helpers -------------------------------------------------------
void analyzeCompression(const string& text, const string& encoded) {
    int originalBits = (int)text.size() * 8;
    int encodedBits  = (int)encoded.size();
    double ratio     = (double)encodedBits / originalBits * 100.0;
    double savings   = 100.0 - ratio;

    cout << "\n--- Compression Analysis ---\n";
    cout << "Original   : " << text.size() << " chars x 8 bits = " << originalBits << " bits\n";
    cout << "Encoded    : " << encodedBits << " bits\n";
    cout << "Ratio      : " << fixed << setprecision(1) << ratio << "% of original\n";
    cout << "Savings    : " << fixed << setprecision(1) << savings << "%\n";
    cout << "Avg bits/ch: " << fixed << setprecision(3) << (double)encodedBits / text.size() << "\n";
}

void sep(const string& t) {
    cout << "\n" << string(54, '=') << "\n " << t << "\n" << string(54, '=') << "\n";
}

int main() {
    cout << "=== Huffman Encoding (Greedy Algorithm) ===\n";

    HuffmanEncoder enc;

    // -- Demo 1 --
    sep("1. Simple text: \"abracadabra\"");
    string text1 = "abracadabra";
    enc.buildFromText(text1);
    unordered_map<char,int> freq1;
    for (char c : text1) freq1[c]++;
    enc.printCodeTable(freq1);
    string bits1 = enc.encode(text1);
    cout << "\nOriginal : " << text1 << "\n";
    cout << "Encoded  : " << bits1 << "\n";
    cout << "Decoded  : " << enc.decodeStr(bits1) << "\n";
    cout << "Lossless : " << (enc.decodeStr(bits1) == text1 ? "YES" : "NO") << "\n";
    analyzeCompression(text1, bits1);

    // -- Demo 2 --
    sep("2. Classic: \"the quick brown fox jumps over the lazy dog\"");
    string text2 = "the quick brown fox jumps over the lazy dog";
    enc.buildFromText(text2);
    unordered_map<char,int> freq2;
    for (char c : text2) freq2[c]++;
    enc.printCodeTable(freq2);
    string bits2 = enc.encode(text2);
    cout << "\nDecoded  : " << enc.decodeStr(bits2) << "\n";
    cout << "Lossless : " << (enc.decodeStr(bits2) == text2 ? "YES" : "NO") << "\n";
    analyzeCompression(text2, bits2);

    // -- Demo 3: Huffman tree structure --
    sep("3. Tree structure for \"aabbcde\"");
    string text3 = "aabbcde";
    enc.buildFromText(text3);
    unordered_map<char,int> freq3;
    for (char c : text3) freq3[c]++;
    enc.printCodeTable(freq3);
    cout << "\nHuffman Tree:\n";
    enc.printTree();

    // -- Demo 4: Edge cases --
    sep("4. Edge cases");

    // Single character repeated
    string text4 = "aaaaaaa";
    enc.buildFromText(text4);
    string bits4 = enc.encode(text4);
    cout << "Single char 'aaaaaaa': encoded=" << bits4
         << " decoded=" << enc.decodeStr(bits4) << "\n";

    // Two characters
    string text5 = "ababab";
    enc.buildFromText(text5);
    string bits5 = enc.encode(text5);
    cout << "Two chars 'ababab':   encoded=" << bits5
         << " decoded=" << enc.decodeStr(bits5) << "\n";

    // -- Demo 5: Longer text --
    sep("5. Realistic paragraph");
    string text6 = "Huffman coding is a lossless data compression algorithm. "
                   "The idea is to assign variable length codes to input characters, "
                   "with shorter codes assigned to more frequent characters.";
    enc.buildFromText(text6);
    unordered_map<char,int> freq6;
    for (char c : text6) freq6[c]++;
    string bits6 = enc.encode(text6);
    cout << "Text length     : " << text6.size() << " chars\n";
    cout << "Unique chars    : " << freq6.size() << "\n";
    cout << "Lossless decode : " << (enc.decodeStr(bits6) == text6 ? "YES" : "NO") << "\n";
    analyzeCompression(text6, bits6);

    return 0;
}