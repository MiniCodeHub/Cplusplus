#include <iostream>
#include <unordered_map>
#include <string>
using namespace std;

// Node for doubly linked list
template <typename K, typename V>
struct Node {
    K key;
    V value;
    Node* prev;
    Node* next;
    
    Node(K k, V v) : key(k), value(v), prev(nullptr), next(nullptr) {}
};

// LRU Cache with O(1) get and put operations
template <typename K, typename V>
class LRUCache {
private:
    int capacity;
    int size;
    unordered_map<K, Node<K, V>*> cache;
    Node<K, V>* head;  // Most recently used
    Node<K, V>* tail;  // Least recently used
    
    // Add node right after head (most recent position)
    void addNode(Node<K, V>* node) {
        node->next = head->next;
        node->prev = head;
        head->next->prev = node;
        head->next = node;
    }
    
    // Remove node from its current position
    void removeNode(Node<K, V>* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }
    
    // Move node to head (mark as most recently used)
    void moveToHead(Node<K, V>* node) {
        removeNode(node);
        addNode(node);
    }
    
    // Remove tail node (least recently used)
    Node<K, V>* removeTail() {
        Node<K, V>* node = tail->prev;
        removeNode(node);
        return node;
    }
    
public:
    LRUCache(int cap) : capacity(cap), size(0) {
        head = new Node<K, V>(K(), V());
        tail = new Node<K, V>(K(), V());
        head->next = tail;
        tail->prev = head;
    }
    
    ~LRUCache() {
        Node<K, V>* curr = head;
        while (curr) {
            Node<K, V>* next = curr->next;
            delete curr;
            curr = next;
        }
    }
    
    // Get value by key, returns default V() if not found
    V get(K key) {
        if (cache.find(key) == cache.end()) {
            cout << "Cache MISS: " << key << endl;
            return V();  // Not found
        }
        
        Node<K, V>* node = cache[key];
        moveToHead(node);  // Mark as recently used
        cout << "Cache HIT: " << key << " -> " << node->value << endl;
        return node->value;
    }
    
    // Insert or update key-value pair
    void put(K key, V value) {
        if (cache.find(key) != cache.end()) {
            // Update existing key
            Node<K, V>* node = cache[key];
            node->value = value;
            moveToHead(node);
            cout << "Updated: " << key << " = " << value << endl;
        } else {
            // Insert new key
            Node<K, V>* newNode = new Node<K, V>(key, value);
            cache[key] = newNode;
            addNode(newNode);
            size++;
            
            cout << "Inserted: " << key << " = " << value;
            
            // Evict LRU if over capacity
            if (size > capacity) {
                Node<K, V>* lru = removeTail();
                cache.erase(lru->key);
                cout << " | Evicted LRU: " << lru->key;
                delete lru;
                size--;
            }
            cout << endl;
        }
    }
    
    // Display current cache state
    void display() {
        cout << "\n=== Cache State (MRU -> LRU) ===" << endl;
        cout << "Size: " << size << "/" << capacity << endl;
        
        Node<K, V>* curr = head->next;
        int pos = 1;
        while (curr != tail) {
            cout << pos++ << ". [" << curr->key << ": " << curr->value << "]";
            if (curr->next != tail) cout << " -> ";
            curr = curr->next;
        }
        if (size == 0) cout << "(empty)";
        cout << "\n================================\n" << endl;
    }
    
    // Check if key exists
    bool contains(K key) {
        return cache.find(key) != cache.end();
    }
    
    int getSize() { return size; }
    int getCapacity() { return capacity; }
};

// Demo program
int main() {
    cout << "=== LRU Cache Simulation ===" << endl;
    cout << "Creating cache with capacity 3\n" << endl;
    
    LRUCache<string, int> cache(3);
    
    // Test insertions
    cache.put("user:101", 42);
    cache.put("user:202", 88);
    cache.put("user:303", 15);
    cache.display();
    
    // Test access (moves to front)
    cache.get("user:101");
    cache.display();
    
    // Test eviction
    cache.put("user:404", 99);  // Should evict user:202
    cache.display();
    
    // Test update
    cache.put("user:101", 100);
    cache.display();
    
    // Test miss
    cache.get("user:202");
    
    // More operations
    cache.put("user:505", 77);
    cache.put("user:606", 33);
    cache.display();
    
    // String cache demo
    cout << "\n=== String Value Cache ===" << endl;
    LRUCache<int, string> strCache(2);
    strCache.put(1, "Alice");
    strCache.put(2, "Bob");
    strCache.get(1);
    strCache.put(3, "Charlie");  // Evicts Bob
    strCache.display();
    
    return 0;
}