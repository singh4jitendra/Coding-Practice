#include <iostream>
#include <map>
#include <chrono>
#include <thread>

using namespace std;
using namespace std::chrono;

map<int, pair<string, long long>> mymap;
map<long long, int> umap;

int get_avg()
{
    int cnt = 0;
    vector<long long> vt;
    for (auto data : umap)
    {
        vt.push_back(data.first);
    }
 cout << "----- displaying vector ---- " << endl;
    for (auto data : vt)
    {
        cout << data << endl;
    }

    auto now = system_clock::now();
    long long nanos = duration_cast<nanoseconds>(
                          now.time_since_epoch())
                          .count();

    cout << "inside avg : nano : " << nanos <<endl;
    auto it = lower_bound(vt.begin(), vt.end(), nanos);
    cout << "out :" << *it << endl;

    cout << "----- displaying vector after lower bound ---- " << endl;
    for (; it != vt.end(); ++it)
    {
        cout << *it << endl;
        cout << mymap[umap[*it]].first << " -> " << mymap[umap[*it]].second << "\n";
        cnt++;
    }
    return cnt;
}

/**
 * @brief Demonstrates usage of a key-value store with TTL (Time-To-Live) functionality.
 *
 * The function performs the following steps:
 * 1. Calculates a TTL duration of 1 minute in nanoseconds.
 * 2. Stores key-value pairs in `mymap` and `umap` with expiration times based on the current time plus TTL.
 * 3. Inserts two entries, waits for 100 seconds, then inserts two more entries.
 * 4. Prints the contents of both maps.
 * 5. Calls and prints the result of `get_avg()`.
 *
 * Assumes the existence of:
 * - `mymap`: a map storing key to a pair of string and expiration timestamp.
 * - `umap`: a map storing expiration timestamp to key.
 * - `get_avg()`: a function returning an average value (purpose unspecified).
 *
 * Uses C++ chrono utilities for time calculations and thread sleep.
 */
int main()
{
    long long ttl = duration_cast<nanoseconds>(minutes(1)).count();

    auto now = system_clock::now();
    long long nanos = duration_cast<nanoseconds>(
                          now.time_since_epoch())
                          .count();

    mymap[1] = {"305", nanos + ttl};
    umap[nanos + ttl] = 1;

    now = system_clock::now();
    nanos = duration_cast<nanoseconds>(
                now.time_since_epoch())
                .count();

    mymap[2] = {"306", nanos + ttl};
    umap[nanos + ttl] = 2;

    this_thread::sleep_for(seconds(100));

    now = system_clock::now();
    nanos = duration_cast<nanoseconds>(
                now.time_since_epoch())
                .count();

    mymap[3] = {"307", nanos + ttl};
    umap[nanos + ttl] = 3;

    now = system_clock::now();
    nanos = duration_cast<nanoseconds>(
                now.time_since_epoch())
                .count();

    mymap[4] = {"308", nanos + ttl};
    umap[nanos + ttl] = 4;

    cout << " ---- map data ---- " <<endl;
    for (auto um : mymap)
    {
        cout <<um.first << " : [" << um.second.first << ", "<<um.second.second << " ]"<<endl;
    }
    
    for (auto um : umap)
    {
        cout << um.first << " : " << um.second << endl;
    }

    cout << get_avg() << endl;
    return 0;
}