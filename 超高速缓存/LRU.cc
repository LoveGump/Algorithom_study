#include <iostream>
#include <unordered_map>
#include <list>
using namespace std;

/**
 * @brief LRU缓存
 * @tparam KeyType 键类型
 * @tparam ValueType 值类型
 */
template<typename KeyType, typename ValueType>
class LRUCache {
public:
    /**
     * @brief 构造函数，初始化缓存容量
     * @param capacity 缓存容量
     */
    LRUCache(size_t capacity) : capacity_(capacity) {}

    /**
     * @brief 获取缓存中的值
     * @param key 键
     * @return 值
     */
    ValueType get(const KeyType& key) {
        auto it = cache_map_.find(key);
        if (it == cache_map_.end()) {
            miss_count_++;
            throw runtime_error("Key not found");
        }
        // 将访问的节点移动到列表的前端（最近使用）
        cache_list_.splice(cache_list_.begin(), cache_list_, it->second);
        return it->second->second;
    }

    /**
     * @brief 插入新的键值对
     * @param key 键
     * @param value 值
     */
    void put(const KeyType& key, const ValueType& value) {
        auto it = cache_map_.find(key);
        if (it != cache_map_.end()) {
            // 更新值并移动到列表的前端
            it->second->second = value;
            cache_list_.splice(cache_list_.begin(), cache_list_, it->second);
        } else {
            miss_count_++;
            // 插入新的节点
            if (cache_list_.size() == capacity_) {
                // 移除最近最少使用的节点
                auto lru = cache_list_.back();
                cache_map_.erase(lru.first);
                cache_list_.pop_back();
            }
            cache_list_.emplace_front(key, value);
            cache_map_[key] = cache_list_.begin();
        }
    }

    /**
     * @brief 显示缓存内容
     */
    void display() const {
        for (const auto& pair : cache_list_) {
            cout << pair.first << ":" << pair.second << " ";
        }
        cout << endl;
    }

    /**
     * @brief 获取缺页次数
     */
    size_t get_miss_count() const {
        return miss_count_;
    }

private:
    size_t capacity_;  // 缓存容量
    list<pair<KeyType, ValueType>> cache_list_;  // 缓存列表，靠近头部的是最近使用

    // 缓存映射，key -> 缓存列表中的迭代器
    unordered_map<KeyType, typename list<pair<KeyType, ValueType>>::iterator> cache_map_;
    size_t miss_count_ = 0; // 缺页次数
};

int main() {
    size_t capacity;
    cout << "请输入缓存容量: ";
    cin >> capacity;
    LRUCache<int, int> cache(capacity);
    cout << "请输入操作序列，每行一个操作（put key value/get key），输入end结束：" << endl;
    string op;
    while (cin >> op) {
        if (op == "end") break;
        if (op == "put") {
            int key, value;
            cin >> key >> value;
            cache.put(key, value);
            cout << "put(" << key << ", " << value << ")\t";
        } else if (op == "get") {
            int key;
            cin >> key;
            try {
                int value = cache.get(key);
                cout << "get(" << key << ") = " << value << "\t";
            } catch (...) {
                cout << "get(" << key << ") = miss\t";
            }
        } else {
            cout << "未知操作: " << op << endl;
            continue;
        }
        cache.display();
        cout << "Miss count: " << cache.get_miss_count() << endl;
    }
    return 0;
}