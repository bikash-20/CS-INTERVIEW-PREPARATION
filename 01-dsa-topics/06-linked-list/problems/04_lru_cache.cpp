// Problem: LRU Cache (LeetCode #146)
// Difficulty: Medium
// Approach: Hash map (key -> list iterator) + doubly linked list (for O(1) move).
//           get(key): if present, move to front, return value.
//           put(key, val): if present, update and move to front.
//                          else add to front. If over capacity, evict tail.
// Time:  O(1) for both ops, Space: O(capacity)

#include <iostream>
#include <unordered_map>
using namespace std;

class LRUCache {
    struct Node {
        int key, val;
        Node* prev, *next;
        Node(int k, int v) : key(k), val(v), prev(nullptr), next(nullptr) {}
    };

    unordered_map<int, Node*> cache;
    Node* head; // dummy head (newest next to it)
    Node* tail; // dummy tail (oldest next to it)
    int cap;

    void remove(Node* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }
    void insertFront(Node* node) {
        node->next = head->next;
        node->prev = head;
        head->next->prev = node;
        head->next = node;
    }

public:
    LRUCache(int capacity) : cap(capacity) {
        head = new Node(0, 0);
        tail = new Node(0, 0);
        head->next = tail;
        tail->prev = head;
    }

    int get(int key) {
        if (!cache.count(key)) return -1;
        Node* node = cache[key];
        remove(node);
        insertFront(node);
        return node->val;
    }

    void put(int key, int value) {
        if (cache.count(key)) {
            cache[key]->val = value;
            remove(cache[key]);
            insertFront(cache[key]);
        } else {
            if ((int)cache.size() == cap) {
                Node* lru = tail->prev;
                cache.erase(lru->key);
                remove(lru);
                delete lru;
            }
            Node* node = new Node(key, value);
            cache[key] = node;
            insertFront(node);
        }
    }
};

int main() {
    LRUCache cache(2);
    cache.put(1, 1);
    cache.put(2, 2);
    cout << cache.get(1) << endl;   // 1
    cache.put(3, 3);                // evicts key 2
    cout << cache.get(2) << endl;   // -1
    cout << cache.get(3) << endl;   // 3
    return 0;
}
