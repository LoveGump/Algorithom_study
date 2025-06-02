#include <iostream>
#include <unordered_map>
#include <deque>
#include <vector>
#include <limits>
using namespace std;


/**
 * @brief LRU_K缓存
 * @tparam KeyType 键类型
 * @tparam ValueType 值类型
 */
template<typename KeyType, typename ValueType>
class LRUKCache {
public:
    /**
     * @brief 构造函数，初始化缓存容量和K值
     * @param capacity 缓存容量
     * @param k K值
     */
    LRUKCache(size_t capacity, size_t k) : capacity_(capacity), k_(k), timestamp_(0) {}

    /**
     * @brief 获取缓存中的值
     * @param key 键
     * @return 值
     */
    ValueType get(const KeyType& key) {
        timestamp_++;
        auto it = cache_map_.find(key);
        if (it == cache_map_.end()) {
            miss_count_++;
            cout << "key not found!!!" << endl;
            return ValueType();
        }
        // 更新访问记录
        updateAccessRecord(key);
        return it->second;
    }

    /**
     * @brief 插入新的键值对
     * @param key 键
     * @param value 值
     */
    void put(const KeyType& key, const ValueType& value) {
        timestamp_++;
        if (cache_map_.find(key) == cache_map_.end()) {
            miss_count_++;
            if (cache_map_.size() == capacity_) {
                evict();
            }
        }
        cache_map_[key] = value;
        updateAccessRecord(key);
    }

    /**
     * @brief 显示缓存内容
     */
    void display() const {
        for (const auto& pair : cache_map_) {
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
    /**
     * @brief 更新访问记录
     * @param key 键
     */
    void updateAccessRecord(const KeyType& key) {
        auto& dq = access_record_[key];
        dq.push_back(timestamp_);  // 将当前时间戳加入访问记录
        if (dq.size() > k_) dq.pop_front();  // 如果访问记录超过K次，移除最早的访问时间
    }

    /**
     * @brief 淘汰策略
     */
    void evict() {
        KeyType lru_key;
        int64_t oldest_kth = numeric_limits<int64_t>::max();
        bool found = false;
        for (const auto& pair : access_record_) {
            if (pair.second.size() == k_) {
                if (pair.second.front() < oldest_kth) {
                    oldest_kth = pair.second.front();
                    lru_key = pair.first;
                    found = true;
                }
            }
        }
        // 如果没有key被访问过K次，选择最早访问的
        if (!found) {
            for (const auto& pair : access_record_) {
                if (pair.second.front() < oldest_kth) {
                    oldest_kth = pair.second.front();
                    lru_key = pair.first;
                }
            }
        }
        cache_map_.erase(lru_key);
        access_record_.erase(lru_key);
    }

    // 缓存容量
    size_t capacity_;
    // K值
    size_t k_;
    // 时间戳
    int64_t timestamp_;
    // 缓存映射，key -> value
    unordered_map<KeyType, ValueType> cache_map_;

    // 访问记录，key -> 访问时间列表
    unordered_map<KeyType, deque<int64_t>> access_record_;
    // 缺页次数
    size_t miss_count_ = 0;
};

int main() {
    size_t capacity, k;
    cout << "请输入缓存容量: ";
    cin >> capacity;
    cout << "请输入K值: ";
    cin >> k;
    LRUKCache<int, int> cache(capacity, k);
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
            int value = cache.get(key);
            cout << "get(" << key << ") = " << value << "\t";
        } else {
            cout << "未知操作: " << op << endl;
            continue;
        }
        cache.display();
        cout << "Miss count: " << cache.get_miss_count() << endl;
    }
    return 0;
}