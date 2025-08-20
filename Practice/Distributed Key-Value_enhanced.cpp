#include <iostream>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <functional>
#include <map>
#include <set>

const int REPLICA_COUNT = 2; // Number of replicas for each key

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

// Consistent Hashing Implementation with Replication and Fault Tolerance
class ConsistentHashing {
private:
    std::map<int, std::string> ring; // Virtual node positions mapped to nodes
    std::hash<std::string> hashFunc; // Hash function for keys
    std::unordered_map<std::string, KeyValueStore> nodeStores; // Store per node
    std::set<std::string> failedNodes; // Track failed nodes

public:
    void addNode(const std::string& nodeName) {
        int hash = hashFunc(nodeName);
        ring[hash] = nodeName;
        nodeStores[nodeName] = KeyValueStore(); // Create store for the node
    }

    void removeNode(const std::string& nodeName) {
        for (auto it = ring.begin(); it != ring.end(); ) {
            if (it->second == nodeName) {
                it = ring.erase(it);
            } else {
                ++it;
            }
        }
        nodeStores.erase(nodeName);
        failedNodes.insert(nodeName);
    }

    std::string getNode(const std::string& key) {
        int keyHash = hashFunc(key);
        auto it = ring.lower_bound(keyHash);
        if (it == ring.end()) {
            return ring.begin()->second; // Wrap around to first node
        }
        return it->second;
    }

    std::string getBalancedNode(const std::string& key) {
        for (auto& [hash, node] : ring) {
            if (failedNodes.find(node) == failedNodes.end()) {
                return node; // Return first available node
            }
        }
        return "No Available Nodes";
    }

    void put(const std::string& key, const std::string& value) {
        int count = 0;
        for (auto it = ring.lower_bound(hashFunc(key)); count < REPLICA_COUNT; ++it, ++count) {
            if (it == ring.end()) it = ring.begin(); // Wrap around
            if (failedNodes.find(it->second) == failedNodes.end()) {
                nodeStores[it->second].put(key, value);
            } else {
                std::string backupNode = getBalancedNode(key);
                if (backupNode != "No Available Nodes") {
                    nodeStores[backupNode].put(key, value);
                }
            }
        }
    }

    std::string get(const std::string& key) {
        int count = 0;
        for (auto it = ring.lower_bound(hashFunc(key)); count < REPLICA_COUNT; ++it, ++count) {
            if (it == ring.end()) it = ring.begin(); // Wrap around
            if (failedNodes.find(it->second) == failedNodes.end()) {
                std::string value = nodeStores[it->second].get(key);
                if (value != "Key Not Found") return value;
            }
        }
        return "Key Not Found";
    }

    void remove(const std::string& key) {
        int count = 0;
        for (auto it = ring.lower_bound(hashFunc(key)); count < REPLICA_COUNT; ++it, ++count) {
            if (it == ring.end()) it = ring.begin(); // Wrap around
            if (failedNodes.find(it->second) == failedNodes.end()) {
                nodeStores[it->second].remove(key);
            }
        }
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
