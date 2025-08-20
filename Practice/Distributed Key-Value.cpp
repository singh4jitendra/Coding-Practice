#include <iostream>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <functional>
#include <map>


/*
✅ Current Code Implements

Consistent Hashing
Thread-Safe Key-Value Store
Basic CRUD Operations
*/

// Thread-safe Key-Value Store with Consistent Hashing
class KeyValueStore {
private:
    std::unordered_map<std::string, std::string> store;
    mutable std::shared_mutex rw_mutex; // Read-Write Lock

public:
    void put(const std::string& key, const std::string& value) {
        std::unique_lock lock(rw_mutex);
        store[key] = value;
    }

    std::string get(const std::string& key) {
        std::shared_lock lock(rw_mutex);
        if (store.find(key) != store.end()) {
            return store[key];
        }
        return "Key Not Found";
    }

    void remove(const std::string& key) {
        std::unique_lock lock(rw_mutex);
        store.erase(key);
    }
};

// Consistent Hashing Implementation
class ConsistentHashing {
private:
    std::map<int, std::string> ring; // Virtual node positions mapped to nodes
    std::hash<std::string> hashFunc; // Hash function for keys
    std::unordered_map<std::string, KeyValueStore> nodeStores; // Store per node

public:
    void addNode(const std::string& nodeName) {
        int hash = hashFunc(nodeName);
        ring[hash] = nodeName;
        nodeStores[nodeName] = KeyValueStore(); // Create store for the node
    }

    std::string getNode(const std::string& key) {
        int keyHash = hashFunc(key);
        auto it = ring.lower_bound(keyHash);
        if (it == ring.end()) {
            return ring.begin()->second; // Wrap around to first node
        }
        return it->second;
    }

    void put(const std::string& key, const std::string& value) {
        std::string node = getNode(key);
        nodeStores[node].put(key, value);
    }

    std::string get(const std::string& key) {
        std::string node = getNode(key);
        return nodeStores[node].get(key);
    }

    void remove(const std::string& key) {
        std::string node = getNode(key);
        nodeStores[node].remove(key);
    }
};

int main() {
    ConsistentHashing ch;
    ch.addNode("NodeA");
    ch.addNode("NodeB");
    ch.addNode("NodeC");

    ch.put("user1", "Alice");
    ch.put("user2", "Bob");

    std::cout << "Get user1: " << ch.get("user1") << std::endl;
    ch.remove("user1");
    std::cout << "Get user1 after delete: " << ch.get("user1") << std::endl;
    
    std::cout << "Key 'user1' assigned to: " << ch.getNode("user1") << std::endl;
    std::cout << "Key 'user2' assigned to: " << ch.getNode("user2") << std::endl;
    
    return 0;
}
