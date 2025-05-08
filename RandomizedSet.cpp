// filepath: ~/code/Algorithm/RandomizedSet.cpp
// link:
#include <algorithm>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <vector>
using namespace std;

class RandomizedSet {
    int size;
    vector<int> arr;
    // 前面是 value 后面是 index
    unordered_map<int, int> map;
    // 创建随机数生成器

   public:
    RandomizedSet() { size = 0; }
    // 当元素 val 不存在时，向集合中插入该项，并返回 true ；否则，返回 false 。
    bool insert(int val) {
        auto it = map.find(val);
        if (it != map.end()) return false;
        arr.push_back(val);
        map[val] = size++;
        return true;
    }

    bool remove(int val) {
        auto it = map.find(val);
        if (it == map.end()) return false;

        // 将最后一个元素移到被删除元素的位置
        int lastElement = arr[--size];
        arr[it->second] = lastElement;

        // 更新最后一个元素的索引
        map[lastElement] = it->second;

        map.erase(val);
        arr.pop_back();
        return true;
    }

    int getRandom() {
        if (size == 0) return -1;  // 处理空集合的情况

        // 生成[0, size-1]范围内的随机索引
        int index = rand() % size;

        // 返回随机元素
        return arr[index];
    }
};

/**
 * Your RandomizedSet object will be instantiated and called as such:
 * RandomizedSet* obj = new RandomizedSet();
 * bool param_1 = obj->insert(val);
 * bool param_2 = obj->remove(val);
 * int param_3 = obj->getRandom();
 */
