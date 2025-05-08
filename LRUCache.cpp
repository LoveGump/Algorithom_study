// filepath: ~/code/Algorithm/LRUCache.cpp
// link:https://leetcode.cn/problems/lru-cache/
#include <algorithm>
#include <iostream>
#include <list>
#include <map>;
#include <string>
#include <vector>
using namespace std;

class LRUCache {
   public:
    int capacity;
    list<pair<int, int>> cache;
    unordered_map<int, list<pair<int, int>>::iterator> map;

    LRUCache(int capacity) : capacity(capacity) {}

    int get(int key) {
        auto it = map.find(key);
        if (it == map.end()) return -1;
        cache.splice(cache.begin(), cache, it->second);
        return it->second->second;
    }

    void put(int key, int value) {
        auto it = map.find(key);

        if (it != map.end()) {
            cache.splice(cache.begin(), cache, it->second);
            it->second->second = value;
            return;
        }
        // 如果没找到
        if (cache.size() == capacity) {
            map.erase(cache.back().first);
            cache.pop_back();
        }
        cache.push_front({key, value});
        map[key] = cache.begin();
    }
};

/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache* obj = new LRUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */
