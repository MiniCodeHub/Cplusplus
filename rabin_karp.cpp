#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <chrono>
using namespace std;

// -- Rabin-Karp String Matching ---------------------------------------------
// Rolling hash: hash = (c1*b^(m-1) + c2*b^(m-2) + ... + cm) % q
class RabinKarp {
private:
    static const int BASE  = 256;    // alphabet size (extended ASCII)
    static const long long MOD  = 1000000007LL;  // large prime

    long long rollingHash(const string& s, int len) {
        long long h = 0;
        for (int i = 0; i < len; i++)
            h = (h * BASE + (unsigned char)s[i]) % MOD;
        return h;
    }

public:
    // Single pattern search -- returns all starting positions (0-indexed)
    vector<int> search(const string& text, const string& pattern) {
        vector<int> matches;
        int n = (int)text.size();
        int m = (int)pattern.size();
        if (m == 0 || m > n) return matches;

        // h = BASE^(m-1) % MOD  (highest coefficient)
        long long h = 1;
        for (int i = 0; i < m - 1; i++) h = (h * BASE) % MOD;

        long long patHash  = rollingHash(pattern, m);
        long long textHash = rollingHash(text, m);

        for (int i = 0; i <= n - m; i++) {
            if (textHash == patHash) {
                // Hash match -- verify character by character (avoid spurious hits)
                if (text.substr(i, m) == pattern)
                    matches.push_back(i);
            }
            // Slide window
            if (i < n - m) {
                textHash = (BASE * (textHash - (unsigned char)text[i] * h % MOD + MOD) % MOD
                            + (unsigned char)text[i + m]) % MOD;
            }
        }
        return matches;
    }

    // Multi-pattern search (search several patterns in one pass each)
    vector<pair<string,vector<int>>> searchMulti(const string& text, const vector<string>& patterns) {
        vector<pair<string,vector<int>>> results;
        for (const auto& p : patterns)
            results.push_back({p, search(text, p)});
        return results;
    }

    // Count occurrences (including overlapping)
    int count(const string& text, const string& pattern) {
        return (int)search(text, pattern).size();
    }
};

// -- Naive brute-force (for comparison) ------------------------------------
vector<int> naiveSearch(const string& text, const string& pattern) {
    vector<int> res;
    int n = (int)text.size(), m = (int)pattern.size();
    for (int i = 0; i <= n - m; i++)
        if (text.substr(i, m) == pattern) res.push_back(i);
    return res;
}

// -- KMP (for comparison) --------------------------------------------------
vector<int> kmpSearch(const string& text, const string& pattern) {
    int n = (int)text.size(), m = (int)pattern.size();
    if (m == 0) return {};
    // Build failure function
    vector<int> fail(m, 0);
    for (int i = 1; i < m; i++) {
        int j = fail[i-1];
        while (j > 0 && pattern[i] != pattern[j]) j = fail[j-1];
        if (pattern[i] == pattern[j]) j++;
        fail[i] = j;
    }
    vector<int> res;
    int j = 0;
    for (int i = 0; i < n; i++) {
        while (j > 0 && text[i] != pattern[j]) j = fail[j-1];
        if (text[i] == pattern[j]) j++;
        if (j == m) { res.push_back(i - m + 1); j = fail[j-1]; }
    }
    return res;
}

// -- Visualise match positions ----------------------------------------------
void visualizeMatches(const string& text, const string& pattern, const vector<int>& positions) {
    const int LINEWIDTH = 72;
    if (text.size() <= (size_t)LINEWIDTH) {
        cout << "  Text:    " << text << "\n";
        string mark(text.size(), ' ');
        for (int pos : positions)
            for (int k = 0; k < (int)pattern.size(); k++) mark[pos + k] = '^';
        cout << "  Matches: " << mark << "\n";
    } else {
        for (int pos : positions) {
            int lo = max(0, pos - 5);
            int hi = min((int)text.size(), pos + (int)pattern.size() + 5);
            cout << "  ...\"" << text.substr(lo, hi - lo) << "\"... at [" << pos << "]\n";
        }
    }
}

void printVec(const vector<int>& v) {
    cout << "[";
    for (size_t i = 0; i < v.size(); i++) cout << v[i] << (i+1<v.size()?",":"");
    cout << "]";
}

void sep(const string& t) {
    cout << "\n" << string(54, '=') << "\n " << t << "\n" << string(54, '=') << "\n";
}

int main() {
    cout << "=== Rabin-Karp String Matching Algorithm ===\n";

    RabinKarp rk;

    // -- Demo 1: Basic search ----------------------------------------------
    sep("1. Basic Search");
    string t1 = "AABAACAADAABAABA";
    string p1 = "AABA";
    auto m1 = rk.search(t1, p1);
    cout << "Text:    \"" << t1 << "\"\n";
    cout << "Pattern: \"" << p1 << "\"\n";
    cout << "Matches at: "; printVec(m1); cout << "\n";
    visualizeMatches(t1, p1, m1);

    // -- Demo 2: Multiple occurrences --------------------------------------
    sep("2. Multiple & Overlapping Matches");
    string t2 = "AAAAAA";
    string p2 = "AAA";
    auto m2 = rk.search(t2, p2);
    cout << "Text: \"" << t2 << "\"  Pattern: \"" << p2 << "\"\n";
    cout << "Matches: "; printVec(m2); cout << "\n";
    visualizeMatches(t2, p2, m2);

    // -- Demo 3: Real text -------------------------------------------------
    sep("3. Natural Language Text");
    string t3 = "the quick brown fox jumps over the lazy dog near the riverbank";
    vector<string> patterns = {"the", "fox", "lazy", "cat", "over the"};
    cout << "Text: \"" << t3 << "\"\n\n";
    for (const auto& p : patterns) {
        auto matches = rk.search(t3, p);
        cout << "  \"" << p << "\" -> ";
        if (matches.empty()) cout << "not found\n";
        else { printVec(matches); cout << " (" << matches.size() << " match" << (matches.size()>1?"es":"") << ")\n"; }
    }

    // -- Demo 4: DNA sequence ----------------------------------------------
    sep("4. DNA Sequence Matching");
    string dna = "ACGTACGTACGTATCGATCGACGTACGT";
    string gene1 = "ACGT";
    string gene2 = "ATCG";
    auto dm1 = rk.search(dna, gene1);
    auto dm2 = rk.search(dna, gene2);
    cout << "DNA: \"" << dna << "\"\n\n";
    cout << "Gene \"" << gene1 << "\": "; printVec(dm1); cout << " (" << dm1.size() << " occurrences)\n";
    visualizeMatches(dna, gene1, dm1);
    cout << "\nGene \"" << gene2 << "\": "; printVec(dm2); cout << " (" << dm2.size() << " occurrences)\n";
    visualizeMatches(dna, gene2, dm2);

    // -- Demo 5: Edge cases ------------------------------------------------
    sep("5. Edge Cases");
    auto e1 = rk.search("hello", "");
    cout << "Empty pattern: matches=" << e1.size() << "\n";
    auto e2 = rk.search("", "abc");
    cout << "Empty text: matches=" << e2.size() << "\n";
    auto e3 = rk.search("ab", "abc");
    cout << "Pattern longer than text: matches=" << e3.size() << "\n";
    auto e4 = rk.search("aaaa", "aaaa");
    cout << "Exact match: "; printVec(e4); cout << "\n";
    auto e5 = rk.search("abcdefgh", "xyz");
    cout << "Pattern not present: matches=" << e5.size() << "\n";

    // -- Demo 6: Algorithm comparison --------------------------------------
    sep("6. Rabin-Karp vs Naive vs KMP");
    string bigText(10000, 'a');
    bigText += "banana"; bigText += string(10000, 'b'); bigText += "banana";
    string pat = "banana";

    auto bench = [&](const string& name, auto fn) {
        auto start = chrono::high_resolution_clock::now();
        auto res   = fn(bigText, pat);
        auto end   = chrono::high_resolution_clock::now();
        double ms  = chrono::duration<double,milli>(end-start).count();
        cout << "  " << left << setw(14) << name
             << " matches=" << res.size()
             << "  time=" << fixed << setprecision(3) << ms << "ms\n";
        return res;
    };

    auto rkRes    = bench("Rabin-Karp", [&](const string& t, const string& p){ return rk.search(t,p); });
    auto naiveRes = bench("Naive",      naiveSearch);
    auto kmpRes   = bench("KMP",        kmpSearch);
    cout << "\n  Results agree: "
         << (rkRes == naiveRes && naiveRes == kmpRes ? "YES" : "NO") << "\n";

    // -- Demo 7: Multi-pattern ---------------------------------------------
    sep("7. Multi-Pattern Search");
    string corpus = "To be or not to be, that is the question. Whether tis nobler.";
    vector<string> keywords = {"be", "to", "the", "noble", "tis"};
    cout << "Text: \"" << corpus << "\"\n\n";
    auto multi = rk.searchMulti(corpus, keywords);
    for (auto& [pat2, positions] : multi) {
        cout << "  \"" << pat2 << "\": ";
        if (positions.empty()) cout << "not found";
        else { printVec(positions); cout << " (" << positions.size() << ")"; }
        cout << "\n";
    }

    return 0;
}