#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <functional>
#include <stdexcept>
#include <iomanip>
#include <algorithm>
#include <numeric>
using namespace std;

// -- Generic HashMap (separate chaining) -----------------------------------
template<typename K, typename V>
class HashMap {
public:
    struct Entry {
        K key;
        V value;
        Entry(K k, V v) : key(move(k)), value(move(v)) {}
    };

private:
    vector<list<Entry>>  buckets;
    size_t               sz;
    size_t               capacity;
    float                maxLoad;

    size_t hashKey(const K& key) const {
        return hash<K>{}(key) % capacity;
    }

    void rehash() {
        size_t newCap = capacity * 2;
        vector<list<Entry>> newBuckets(newCap);
        for (auto& chain : buckets)
            for (auto& e : chain)
                newBuckets[hash<K>{}(e.key) % newCap].emplace_back(e.key, e.value);
        buckets  = move(newBuckets);
        capacity = newCap;
        cout << "  [rehash] -> capacity: " << capacity << ", load: "
             << fixed << setprecision(2) << loadFactor() << "\n";
    }

public:
    explicit HashMap(size_t cap = 8, float maxLF = 0.75f)
        : buckets(cap), sz(0), capacity(cap), maxLoad(maxLF) {}

    // Insert or update
    void put(const K& key, const V& value) {
        if ((float)(sz + 1) / capacity > maxLoad) rehash();
        size_t i = hashKey(key);
        for (auto& e : buckets[i]) {
            if (e.key == key) { e.value = value; return; }
        }
        buckets[i].emplace_back(key, value);
        sz++;
    }

    // Get by key (throws if not found)
    V& get(const K& key) {
        size_t i = hashKey(key);
        for (auto& e : buckets[i])
            if (e.key == key) return e.value;
        throw out_of_range("Key not found.");
    }

    const V& get(const K& key) const {
        size_t i = hashKey(key);
        for (const auto& e : buckets[i])
            if (e.key == key) return e.value;
        throw out_of_range("Key not found.");
    }

    // Get with default (no throw)
    V getOrDefault(const K& key, const V& def) const {
        size_t i = hashKey(key);
        for (const auto& e : buckets[i])
            if (e.key == key) return e.value;
        return def;
    }

    bool contains(const K& key) const {
        size_t i = hashKey(key);
        for (const auto& e : buckets[i])
            if (e.key == key) return true;
        return false;
    }

    bool remove(const K& key) {
        size_t i = hashKey(key);
        auto& chain = buckets[i];
        for (auto it = chain.begin(); it != chain.end(); ++it) {
            if (it->key == key) { chain.erase(it); sz--; return true; }
        }
        return false;
    }

    // Subscript: insert default if absent
    V& operator[](const K& key) {
        if (!contains(key)) put(key, V{});
        return get(key);
    }

    size_t size()     const { return sz; }
    bool   empty()    const { return sz == 0; }
    float  loadFactor() const { return (float)sz / capacity; }
    size_t bucketCount() const { return capacity; }

    // All keys
    vector<K> keys() const {
        vector<K> result;
        for (const auto& chain : buckets)
            for (const auto& e : chain) result.push_back(e.key);
        return result;
    }

    // All values
    vector<V> values() const {
        vector<V> result;
        for (const auto& chain : buckets)
            for (const auto& e : chain) result.push_back(e.value);
        return result;
    }

    // All entries
    vector<Entry> entries() const {
        vector<Entry> result;
        for (const auto& chain : buckets)
            for (const auto& e : chain) result.push_back(e);
        return result;
    }

    // Iterator-style forEach
    void forEach(function<void(const K&, const V&)> fn) const {
        for (const auto& chain : buckets)
            for (const auto& e : chain) fn(e.key, e.value);
    }

    // Stats
    void printStats() const {
        int usedBuckets = 0, maxChain = 0, totalChain = 0;
        vector<int> distribution(10, 0);
        for (const auto& chain : buckets) {
            int len = (int)chain.size();
            if (len > 0) usedBuckets++;
            maxChain   = max(maxChain, len);
            totalChain += len;
            if (len < 10) distribution[len]++;
        }
        cout << "  Size        : " << sz << "\n";
        cout << "  Capacity    : " << capacity << "\n";
        cout << "  Load factor : " << fixed << setprecision(3) << loadFactor() << "\n";
        cout << "  Used buckets: " << usedBuckets << " / " << capacity << "\n";
        cout << "  Max chain   : " << maxChain << "\n";
        cout << "  Avg chain   : " << fixed << setprecision(2)
             << (usedBuckets > 0 ? (float)totalChain / usedBuckets : 0.0f) << "\n";
        cout << "  Distribution (chain_len: count):\n";
        for (int i = 0; i < 10; i++)
            if (distribution[i] > 0) cout << "    len=" << i << ": " << distribution[i] << " bucket(s)\n";
    }

    void printAll() const {
        auto es = entries();
        sort(es.begin(), es.end(), [](const Entry& a, const Entry& b){
            if constexpr (is_same_v<K,string>) return a.key < b.key;
            else return a.key < b.key;
        });
        cout << "{ ";
        for (size_t i = 0; i < es.size(); i++) {
            cout << es[i].key << ": " << es[i].value;
            if (i + 1 < es.size()) cout << ", ";
        }
        cout << " }\n";
    }
};

// -- Applications -----------------------------------------------------------

// Word frequency counter
HashMap<string,int> wordFrequency(const string& text) {
    HashMap<string,int> freq;
    string word;
    for (char c : text) {
        if (isalpha(c)) word += tolower(c);
        else if (!word.empty()) { freq[word]++; word.clear(); }
    }
    if (!word.empty()) freq[word]++;
    return freq;
}

// Two-sum using hashmap
pair<int,int> twoSum(const vector<int>& nums, int target) {
    HashMap<int,int> seen;  // value -> index
    for (int i = 0; i < (int)nums.size(); i++) {
        int complement = target - nums[i];
        if (seen.contains(complement)) return {seen.get(complement), i};
        seen.put(nums[i], i);
    }
    return {-1,-1};
}

// Group anagrams
vector<vector<string>> groupAnagrams(const vector<string>& words) {
    HashMap<string, vector<string>> groups;
    for (const auto& w : words) {
        string key = w;
        sort(key.begin(), key.end());
        groups[key].push_back(w);
    }
    vector<vector<string>> result;
    groups.forEach([&](const string&, const vector<string>& v){ result.push_back(v); });
    return result;
}

void sep(const string& t) {
    cout << "\n" << string(52, '-') << "\n " << t << "\n" << string(52, '-') << "\n";
}

int main() {
    cout << "=== HashMap: Separate Chaining ===\n";

    sep("1. Basic Operations");
    HashMap<string,int> map;
    map.put("apple", 5); map.put("banana", 3); map.put("cherry", 8);
    map.put("date", 2);  map.put("elderberry", 1);
    cout << "Map: "; map.printAll();
    cout << "apple  = " << map.get("apple")  << "\n";
    cout << "banana = " << map.get("banana") << "\n";
    cout << "contains(cherry) = " << (map.contains("cherry") ? "YES" : "NO") << "\n";
    cout << "contains(grape)  = " << (map.contains("grape")  ? "YES" : "NO") << "\n";

    sep("2. Update & Remove");
    map.put("apple", 99);
    cout << "After update apple=99: apple = " << map.get("apple") << "\n";
    map.remove("banana");
    cout << "After remove banana: "; map.printAll();
    cout << "size = " << map.size() << "\n";

    sep("3. Subscript operator []");
    HashMap<string,int> counter;
    counter["cats"]++; counter["cats"]++; counter["dogs"]++;
    counter["cats"]++; counter["birds"]++;
    cout << "cats="  << counter["cats"]
         << " dogs=" << counter["dogs"]
         << " birds="<< counter["birds"] << "\n";

    sep("4. getOrDefault");
    cout << "getOrDefault(cats, 0)    = " << counter.getOrDefault("cats",0)  << "\n";
    cout << "getOrDefault(fish, 0)    = " << counter.getOrDefault("fish",0)  << "\n";
    cout << "getOrDefault(fish, -999) = " << counter.getOrDefault("fish",-999) << "\n";

    sep("5. Rehashing (load factor trigger)");
    HashMap<int,string> rhMap(4, 0.75f);  // tiny initial capacity
    cout << "Initial capacity: 4, inserting 10 items:\n";
    for (int i = 0; i < 10; i++) rhMap.put(i, "val" + to_string(i));
    rhMap.printStats();

    sep("6. Stats on large map");
    HashMap<int,int> bigMap;
    for (int i = 0; i < 200; i++) bigMap.put(i * 7 % 1000, i);
    bigMap.printStats();

    sep("7. Error handling");
    try { map.get("not_a_key"); }
    catch(const out_of_range& e) { cout << "Caught: " << e.what() << "\n"; }

    sep("8. Application: Word Frequency");
    string text = "the quick brown fox jumps over the lazy dog the fox";
    auto freq = wordFrequency(text);
    cout << "Text: \"" << text << "\"\n\n";
    auto fe = freq.entries();
    sort(fe.begin(), fe.end(), [](auto& a, auto& b){ return a.value > b.value; });
    for (auto& e : fe)
        cout << "  " << left << setw(12) << e.key << ": " << e.value << "\n";

    sep("9. Application: Two-Sum");
    vector<int> nums = {2, 7, 11, 15, 1, 8, 3};
    cout << "Array: [2, 7, 11, 15, 1, 8, 3]\n";
    for (int t : {9, 18, 4, 100}) {
        auto [i,j] = twoSum(nums, t);
        cout << "target=" << t << ": ";
        if (i == -1) cout << "No solution\n";
        else cout << "indices [" << i << ", " << j << "] -> " << nums[i] << " + " << nums[j] << "\n";
    }

    sep("10. Application: Group Anagrams");
    vector<string> words = {"eat","tea","tan","ate","nat","bat","arc","car"};
    auto groups = groupAnagrams(words);
    cout << "Input: eat tea tan ate nat bat arc car\n\nGroups:\n";
    for (auto& g : groups) {
        cout << "  [ ";
        for (const auto& w : g) cout << w << " ";
        cout << "]\n";
    }

    return 0;
}