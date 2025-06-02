#include <iostream>
#include <unordered_map>
#include <list>
using namespace std;

/**
 * @brief LFU缓存
 * @tparam KeyType 键类型
 * @tparam ValueType 值类型
 */
template<typename KeyType, typename ValueType>
class LFUCache {
public:
    /**
     * @brief 构造函数，初始化缓存容量
     * @param capacity 缓存容量
     */
    LFUCache(size_t capacity) : capacity_(capacity), min_freq_(0) {}

    /**
     * @brief 获取缓存中的值
     * @param key 键
     * @return 值
     */
    ValueType get(const KeyType& key) {
        if (cache_map_.find(key) == cache_map_.end()) {
            miss_count_++;
            cout << "key not found!!!" << endl;
            return ValueType();
        }
        touch(key);
        return cache_map_[key].first;
    }

    /**
     * @brief 插入新的键值对
     * @param key 键
     * @param value 值
     */
    void put(const KeyType& key, const ValueType& value) {
        if (capacity_ == 0) return;
        if (cache_map_.find(key) != cache_map_.end()) {
            cache_map_[key].first = value;
            touch(key);
            return;
        }
        miss_count_++;
        // 如果缓存满了，淘汰最少频率且最久未使用的key
        if (cache_map_.size() == capacity_) {
            // 淘汰最少频率且最久未使用的key
            auto lfu_key = freq_map_[min_freq_].back(); // 获取最少频率的key
            freq_map_[min_freq_].pop_back(); // 移除该key
            cache_map_.erase(lfu_key); // 移除缓存中的该key
            key_freq_.erase(lfu_key); // 移除key的频率信息
        }
        // 插入新key
        cache_map_[key] = {value, 1}; 
        freq_map_[1].push_front(key); 
        key_iter_[key] = freq_map_[1].begin(); // 更新迭代器
        key_freq_[key] = 1; // 更新key的频率
        min_freq_ = 1; // 更新最小频率
    }

    /**
     * @brief 显示缓存内容
     */
    void display() const {
        for (const auto& pair : cache_map_) {
            cout << pair.first << ":" << pair.second.first << " ";
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
   void touch(const KeyType& key) {
    int freq = key_freq_[key]; // 获取当前key的访问频率
    auto it = key_iter_[key];  // 获取迭代器
    freq_map_[freq].erase(it); // 从当前频率的链表中移除该key

    // 如果当前频率链表为空，并且这个频率是最小频率，则最小频率加1
    if (freq_map_[freq].empty() && freq == min_freq_) {
        min_freq_++;
    }

    // 把key插入到更高频率（freq+1）对应的链表头部
    freq_map_[freq + 1].push_front(key);

    // 更新迭代器
    key_iter_[key] = freq_map_[freq + 1].begin();

    // 更新key的频率
    key_freq_[key] = freq + 1;

    // 更新缓存中key的频率信息
    cache_map_[key].second = freq + 1;
}

    // 缓存容量
    size_t capacity_;
    // 最小频率
    int min_freq_;
    // key -> (value, freq)
    unordered_map<KeyType, pair<ValueType, int>> cache_map_;
    // freq -> list of keys
    unordered_map<int, list<KeyType>> freq_map_;
    // key -> iterator in freq_map_
    unordered_map<KeyType, typename list<KeyType>::iterator> key_iter_;
    // key -> freq
    unordered_map<KeyType, int> key_freq_;
    // 缺页次数
    size_t miss_count_ = 0;
};

int main() {
    size_t capacity;
    cout << "请输入缓存容量: ";
    cin >> capacity;
    LFUCache<int, int> cache(capacity);
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