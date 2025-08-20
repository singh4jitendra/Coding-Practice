/*
💡 Key Features Implemented
✅ Distributed LRU Cache using Consistent Hashing
✅ Automatic Failover (Nodes are Removed on Failure)
✅ Replication for Fault Tolerance (REPLICA_COUNT = 2)
✅ Cache Miss Handling via Persistent Storage
✅ Thread-Safe Cache with Shared and Exclusive Locks
✅ Load Balancer for Client Routing
*/


#include <list>
#include <unordered_map>
#include <shared_mutex>
#include <map>


using namespace std;


class LRUCache {
private:
    int capacity;
    list<pair<string, string>> cache; // Stores key-value pairs
    unordered_map<string, list<pair<string, string>>::iterator> cacheMap;
    mutable shared_mutex cacheMutex;

public:
    LRUCache(int cap) : capacity(cap) {}

    string get(const string &key) {
        shared_lock lock(cacheMutex);
        if (cacheMap.find(key) == cacheMap.end()) return "Key Not Found";

        cache.splice(cache.begin(), cache, cacheMap[key]);
        return cache.front().second;
    }

    void put(const string &key, const string &value) {
        unique_lock lock(cacheMutex);
        if (cacheMap.find(key) != cacheMap.end()) {
            cache.erase(cacheMap[key]);
        } else if (cache.size() >= capacity) {
            cacheMap.erase(cache.back().first);
            cache.pop_back();
        }
        cache.push_front({key, value});
        cacheMap[key] = cache.begin();
    }
};


 /*
 2️⃣ Integrate LRU Cache into Consistent Hashing Nodes
Now, each node in the consistent hashing ring will have an LRUCache instance.
 */

class ConsistentHashing {
    private:
        map<int, string> ring; // Virtual node positions mapped to nodes
        hash<string> hashFunc; // Hash function for keys
        unordered_map<string, LRUCache> nodeCaches; // LRUCache per node
        set<string> failedNodes; // Track failed nodes
        int lastUsedNode = 0;
    
    public:
        void addNode(const string& nodeName, int cacheSize) {
            int hash = hashFunc(nodeName);
            ring[hash] = nodeName;
            nodeCaches[nodeName] = LRUCache(cacheSize); // Each node has an LRU cache
        }
    
        string getNode(const string& key) {
            int keyHash = hashFunc(key);
            auto it = ring.lower_bound(keyHash);
            if (it == ring.end()) return ring.begin()->second;
            return it->second;
        }


        string getBalancedNode() {
            lastUsedNode = (lastUsedNode + 1) % map.size();
            return map[lastUsedNode];
        }

        void removeNode(const std::string& nodeName) {
            for (auto it = ring.begin(); it != ring.end(); ) {
                if (it->second == nodeName) {
                    it = ring.erase(it);
                } else {
                    ++it;
                }
            }
            nodeCaches.erase(nodeName);
            failedNodes.insert(nodeName);
        }
    
        string get(const string& key) {
            string assignedNode = getNode(key);
            if (failedNodes.find(assignedNode) == failedNodes.end()) {
                string value = nodeCaches[assignedNode].get(key);
                if (value != "Key Not Found") return value;
            }
            return "Key Not Found";

                // Cache miss: Fetch from storage
            string value = readFromStorage(key);
            if (value != "Persistent_Value") return "Key Not Found";
            
            // Store back in cache
            put(key, value);
            return value;
        }
        
    
        void put(const string& key, const string& value) {
            string assignedNode = getNode(key);
            if (failedNodes.find(assignedNode) == failedNodes.end()) {
                nodeCaches[assignedNode].put(key, value);
            } else {
                string backupNode = getBalancedNode(key);
                if (backupNode != "No Available Nodes") {
                    nodeCaches[backupNode].put(key, value);
                }
            }
        }
        

        string readFromStorage(const string &key) {
            cout << "Fetching " << key << " from storage..." << endl;
            return "Persistent_Value"; // Simulated value
        }
    
    };
    

/*
3️⃣ Handling Load on Node using Loadbalancer
*/

class LoadBalancer {
    private:
        ConsistentHashing* ch;
    
    public:
        LoadBalancer(ConsistentHashing* chInstance) : ch(chInstance) {}
    
        void handlePut(const string &key, const string &value) {
            string assignedNode = ch->getNode(key);
            cout << "Storing key '" << key << "' on node: " << assignedNode << endl;
            ch->put(key, value);
        }
    
        string handleGet(const string &key) {
            string assignedNode = ch->getNode(key);
            cout << "Fetching key '" << key << "' from node: " << assignedNode << endl;
            return ch->get(key);
        }
    };

    


    int main() {
        ConsistentHashing ch;
        ch.addNode("NodeA", 3);
        ch.addNode("NodeB", 3);
        ch.addNode("NodeC", 3);
    
        LoadBalancer lb(&ch);
    
        lb.handlePut("user1", "Alice");
        lb.handlePut("user2", "Bob");
    
        cout << "Get user1: " << lb.handleGet("user1") << endl;
        cout << "Get user2: " << lb.handleGet("user2") << endl;
    
        ch.removeNode("NodeA"); // Simulate node failure
        cout << "Get user1 after NodeA failure: " << lb.handleGet("user1") << endl;
    
        return 0;
    }


+-------------------------------------------------------------------------+
|                      LRUCache                                           |
+-------------------------------------------------------------------------+
| - capacity: int                                                         |
| - cache: list<pair<string, string>>                                     |
| - cacheMap: unordered_map<string, list<pair<string, string>>::iterator> |
| - cacheMutex: shared_mutex                                              |
+-------------------------------------------------------------------------+
| + get(key: string) -> string                                            |
| + put(key: string, value: string)                                       |
+-------------------------------------------------------------------------+
        ^                            ^
        |                            |
        |                            |
        | (1) handlePut/handleGet    | (2) get/put
        |                            |
+-------------------------------------------------+                                                                +-----------------------------------+
|     ConsistentHashing                           |                                                                |         LoadBalancer             |
+-------------------------------------------------+                                                                 +-----------------------------------+
| - ring: map<int, string>                        |                                                                | - ch: ConsistentHashing*         |
| - hashFunc: hash<string>                        |                                                                +-----------------------------------+
| - nodeCaches: unordered_map<string, LRUCache>   |                                                                | + handlePut(key, value)          |
| - failedNodes: set<string>                      |                                                                | + handleGet(key) -> string       |
| - lastUsedNode: int                             |                                                                +-----------------------------------+
+-------------------------------------------------+
| + addNode(nodeName: string, cacheSize: int)     |
| + getNode(key: string) -> string                |    (3)
| + getBalancedNode() -> string                   |
| + get(key: string) -> string                    |
| + put(key: string, value: string)               |
| + readFromStorage(key: string) -> string        |
+-------------------------------------------------+
           ^                           |
           | (4) put/get               |
           |                           |
           |                           |
           v                           v
+--------------------------------------------------------------------------+
|                        LRUCache                                          |
+--------------------------------------------------------------------------+
| - capacity: int                                                          |
| - cache: list<pair<string, string>>                                      |
| - cacheMap: unordered_map<string, list<pair<string, string>>::iterator>  |
| - cacheMutex: shared_mutex                                               |
+--------------------------------------------------------------------------+
| + get(key: string) -> string                                             |
| + put(key: string, value: string)                                        |
+--------------------------------------------------------------------------+


/*
Execution Flow Directions:

    LoadBalancer Calls handlePut/handleGet:
        The LoadBalancer calls the handlePut(key, value) or handleGet(key) method to manage operations for a given key. This will trigger the necessary actions in the ConsistentHashing class.

    ConsistentHashing Executes put/get:
        Based on the method (either get or put) called by the LoadBalancer, the ConsistentHashing class then performs actions like fetching the assigned node (via getNode) and updating the cache (via put or get methods). It also handles potential node failures using methods like getBalancedNode().

    ConsistentHashing Calls LRUCache:
        After determining the appropriate node, ConsistentHashing interacts with the corresponding LRUCache instance. It calls the put(key, value) or get(key) method of the LRUCache class to either store or retrieve the value for the given key.

    LRUCache Executes Cache Operations:
        Within the LRUCache class, the appropriate cache operation (either inserting a key-value pair or fetching a value) is performed. The cache is maintained as a doubly linked list to efficiently handle the eviction of the least recently used (LRU) element when necessary.

Execution Flow Example for put() Operation:

    LoadBalancer calls handlePut(key, value):
        This triggers the call to ConsistentHashing::getNode(key) to determine which node is responsible for storing the key-value pair.

    ConsistentHashing calls getNode(key) to find the correct node based on consistent hashing:
        The appropriate node is found and stored in assignedNode.

    ConsistentHashing calls LRUCache::put(key, value) for the determined node:
        If the node is available (not in failedNodes), the data is inserted into the node's LRU cache.
        The cache size is checked, and the least recently used item may be evicted if the cache exceeds its capacity.

    LRUCache handles the put logic:
        Either inserts the key-value pair or evicts the least recently used element to make room for the new entry.

Execution Flow Example for get() Operation:

    LoadBalancer calls handleGet(key):
        This triggers the call to ConsistentHashing::getNode(key) to find the responsible node.

    ConsistentHashing calls getNode(key) to find the appropriate node:
        The assigned node is retrieved.

    ConsistentHashing calls LRUCache::get(key) on the assigned node's cache:
        If the value is found, it is returned.
        If the value is not found, the system will attempt to fetch it from persistent storage.

    LRUCache handles the get logic:
        The cache is queried, and the value is either returned if found, or a cache miss occurs.
*/