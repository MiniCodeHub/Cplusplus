#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
using namespace std;

// -- Trie Node --------------------------------------------------------------
struct TrieNode {
    TrieNode* children[26];
    bool      isEnd;
    int       freq;     // how many times this word was inserted
    int       prefixCount; // how many words pass through this node

    TrieNode() : isEnd(false), freq(0), prefixCount(0) {
        fill(children, children + 26, nullptr);
    }
};

// -- Trie -------------------------------------------------------------------
class Trie {
private:
    TrieNode* root;
    int       totalWords;

    int idx(char c) const { return tolower(c) - 'a'; }

    // Helper: collect all words under a node with given prefix
    void collectWords(TrieNode* node, string current, vector<string>& results) const {
        if (!node) return;
        if (node->isEnd) results.push_back(current);
        for (int i = 0; i < 26; i++) {
            if (node->children[i])
                collectWords(node->children[i], current + (char)('a' + i), results);
        }
    }

    // Helper: delete a word (returns true if node can be deleted)
    bool deleteHelper(TrieNode* node, const string& word, int depth) {
        if (!node) return false;
        if (depth == (int)word.size()) {
            if (!node->isEnd) return false;
            node->isEnd = false;
            node->freq  = 0;
            return all_of(node->children, node->children + 26, [](TrieNode* c){ return !c; });
        }
        int i = idx(word[depth]);
        if (deleteHelper(node->children[i], word, depth + 1)) {
            delete node->children[i];
            node->children[i] = nullptr;
            return !node->isEnd &&
                   all_of(node->children, node->children + 26, [](TrieNode* c){ return !c; });
        }
        return false;
    }

    void destroyHelper(TrieNode* node) {
        if (!node) return;
        for (int i = 0; i < 26; i++) destroyHelper(node->children[i]);
        delete node;
    }

    // Print tree structure
    void printHelper(TrieNode* node, const string& prefix, const string& childPrefix) const {
        if (!node) return;
        cout << prefix;
        if (node->isEnd) cout << " [word, freq=" << node->freq << "]";
        cout << "\n";
        vector<int> validChildren;
        for (int i = 0; i < 26; i++)
            if (node->children[i]) validChildren.push_back(i);
        for (int j = 0; j < (int)validChildren.size(); j++) {
            int i = validChildren[j];
            bool last = (j == (int)validChildren.size() - 1);
            printHelper(node->children[i],
                childPrefix + (last ? "`-- " : "|-- ") + (char)('a' + i),
                childPrefix + (last ? "    " : "|   "));
        }
    }

public:
    Trie() : root(new TrieNode()), totalWords(0) {}

    // Insert a word
    void insert(const string& word) {
        TrieNode* curr = root;
        for (char c : word) {
            int i = idx(c);
            if (!curr->children[i]) curr->children[i] = new TrieNode();
            curr = curr->children[i];
            curr->prefixCount++;
        }
        if (!curr->isEnd) totalWords++;
        curr->isEnd = true;
        curr->freq++;
    }

    // Search for exact word
    bool search(const string& word) const {
        TrieNode* curr = root;
        for (char c : word) {
            int i = idx(c);
            if (!curr->children[i]) return false;
            curr = curr->children[i];
        }
        return curr->isEnd;
    }

    // Check if any word starts with prefix
    bool startsWith(const string& prefix) const {
        TrieNode* curr = root;
        for (char c : prefix) {
            int i = idx(c);
            if (!curr->children[i]) return false;
            curr = curr->children[i];
        }
        return true;
    }

    // Count words with given prefix
    int countWithPrefix(const string& prefix) const {
        TrieNode* curr = root;
        for (char c : prefix) {
            int i = idx(c);
            if (!curr->children[i]) return 0;
            curr = curr->children[i];
        }
        return curr->prefixCount;
    }

    // Get word frequency
    int frequency(const string& word) const {
        TrieNode* curr = root;
        for (char c : word) {
            int i = idx(c);
            if (!curr->children[i]) return 0;
            curr = curr->children[i];
        }
        return curr->isEnd ? curr->freq : 0;
    }

    // Autocomplete: all words with given prefix, sorted
    vector<string> autocomplete(const string& prefix) const {
        vector<string> results;
        TrieNode* curr = root;
        for (char c : prefix) {
            int i = idx(c);
            if (!curr->children[i]) return results;
            curr = curr->children[i];
        }
        collectWords(curr, prefix, results);
        sort(results.begin(), results.end());
        return results;
    }

    // Delete a word
    bool remove(const string& word) {
        if (!search(word)) return false;
        // Decrement prefixCounts first
        TrieNode* curr = root;
        for (char c : word) {
            curr = curr->children[idx(c)];
            curr->prefixCount--;
        }
        bool removed = deleteHelper(root, word, 0);
        if (removed || search(word) == false) {
            totalWords--;
            return true;
        }
        return false;
    }

    // Longest common prefix of all words
    string longestCommonPrefix() const {
        string lcp;
        TrieNode* curr = root;
        while (true) {
            int validCount = 0, nextIdx = -1;
            for (int i = 0; i < 26; i++) {
                if (curr->children[i]) { validCount++; nextIdx = i; }
            }
            if (validCount != 1 || curr->isEnd) break;
            lcp += (char)('a' + nextIdx);
            curr = curr->children[nextIdx];
        }
        return lcp;
    }

    int size() const { return totalWords; }

    void printStructure() const {
        cout << "(root)\n";
        printHelper(root, "(root)", "");
    }

    ~Trie() { destroyHelper(root); }
};

// -- Demos ------------------------------------------------------------------
void sep(const string& t) {
    cout << "\n" << string(52, '-') << "\n " << t << "\n" << string(52, '-') << "\n";
}

void printVec(const vector<string>& v, const string& label) {
    cout << label << ": [";
    for (size_t i = 0; i < v.size(); i++)
        cout << v[i] << (i+1 < v.size() ? ", " : "");
    cout << "]\n";
}

int main() {
    cout << "=== Trie Data Structure ===\n";

    Trie trie;

    sep("1. Insert words");
    vector<string> words = {
        "apple","app","application","apply","apt",
        "banana","band","bandana","bank","bat",
        "cat","catch","car","card","care",
        "do","dog","door","done","double"
    };
    for (const auto& w : words) trie.insert(w);
    cout << "Inserted " << words.size() << " words. Trie size: " << trie.size() << "\n";

    sep("2. Exact Search");
    for (const auto& w : {"apple","app","ap","banana","xyz","car","card"}) {
        cout << "search(\"" << w << "\") = "
             << (trie.search(w) ? "FOUND" : "NOT FOUND") << "\n";
    }

    sep("3. Prefix Check");
    for (const auto& p : {"app","ban","xyz","do","cat","z"}) {
        cout << "startsWith(\"" << p << "\") = "
             << (trie.startsWith(p) ? "YES" : "NO") << "\n";
    }

    sep("4. Autocomplete");
    for (const auto& p : {"app","ba","ca","do","b"}) {
        auto results = trie.autocomplete(p);
        printVec(results, "autocomplete(\"" + string(p) + "\")");
    }

    sep("5. Count Words with Prefix");
    for (const auto& p : {"app","ba","ca","do","a","z"}) {
        cout << "countWithPrefix(\"" << p << "\") = "
             << trie.countWithPrefix(p) << "\n";
    }

    sep("6. Word Frequency (insert duplicates)");
    trie.insert("apple"); trie.insert("apple");
    trie.insert("banana");
    cout << "freq(apple)  = " << trie.frequency("apple")  << "\n";
    cout << "freq(banana) = " << trie.frequency("banana") << "\n";
    cout << "freq(cat)    = " << trie.frequency("cat")    << "\n";
    cout << "freq(xyz)    = " << trie.frequency("xyz")    << "\n";

    sep("7. Longest Common Prefix");
    Trie trie2;
    trie2.insert("flower"); trie2.insert("flow"); trie2.insert("flight");
    cout << "Words: flower, flow, flight\n";
    cout << "LCP = \"" << trie2.longestCommonPrefix() << "\"\n";

    Trie trie3;
    trie3.insert("dog"); trie3.insert("racecar"); trie3.insert("car");
    cout << "\nWords: dog, racecar, car\n";
    cout << "LCP = \"" << trie3.longestCommonPrefix() << "\"\n";

    sep("8. Delete a word");
    cout << "Before delete: search(app) = " << (trie.search("app") ? "FOUND" : "NOT FOUND") << "\n";
    trie.remove("app");
    cout << "After  delete: search(app) = " << (trie.search("app") ? "FOUND" : "NOT FOUND") << "\n";
    cout << "apple still present: " << (trie.search("apple") ? "YES" : "NO") << "\n";
    printVec(trie.autocomplete("app"), "autocomplete(app) after deleting 'app'");

    sep("9. Edge Cases");
    Trie edge;
    edge.insert("a");
    cout << "search(a)    = " << (edge.search("a") ? "FOUND" : "NOT FOUND") << "\n";
    cout << "search(\"\")   = " << (edge.search("") ? "FOUND" : "NOT FOUND") << "\n";
    edge.insert("");
    cout << "After insert empty: search(\"\") = " << (edge.search("") ? "FOUND" : "NOT FOUND") << "\n";
    cout << "Size = " << edge.size() << "\n";

    sep("10. Trie Structure (small)");
    Trie vis;
    vis.insert("cat"); vis.insert("car"); vis.insert("card");
    vis.insert("bat"); vis.insert("ball");
    vis.printStructure();

    return 0;
}