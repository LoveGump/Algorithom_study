#include <iostream>
#include <unordered_map>
#include <list>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>

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

// 性能统计结构
struct PerformanceStats {
    size_t hit_count = 0;
    size_t miss_count = 0;
    double total_time = 0.0;
    
    double hit_rate() const {
        return static_cast<double>(hit_count) / (hit_count + miss_count);
    }
    
    double average_access_time() const {
        return total_time / (hit_count + miss_count);
    }
};

// 测试单个访问模式
PerformanceStats test_pattern(const string& pattern_file, size_t capacity) {
    PerformanceStats stats;
    LFUCache<int, int> cache(capacity);
    
    // 读取测试数据
    ifstream file(pattern_file);
    if (!file.is_open()) {
        cerr << "无法打开文件: " << pattern_file << endl;
        return stats;
    }
    
    string line;
    while (getline(file, line)) {
        int key = stoi(line);
        
        // 记录开始时间
        auto start = chrono::high_resolution_clock::now();
        

        cache.put(key, key);  // 使用key作为value
        // 记录结束时间
        auto end = chrono::high_resolution_clock::now();
        stats.total_time += chrono::duration<double>(end - start).count();
    }
    stats.miss_count = cache.get_miss_count();
    stats.hit_count = 100000 - stats.miss_count;
    
    return stats;
}

int main() {
    // 测试参数
    vector<size_t> cache_sizes = {
        16,     // 最小缓存大小
        64,     // 4倍
        256,    // 16倍
        1024,   // 64倍
        4096    // 最大缓存大小
    };
    string test_data_dir = "test_data";
    
    // 创建结果目录
    system("mkdir -p results");
    
    // 对每个缓存大小进行测试
    for (size_t capacity : cache_sizes) {
        cout << "\n测试配置: 缓存大小=" << capacity 
             << " (" << (capacity * 100.0 / 100000) << "% of data space)" << endl;
        cout << "----------------------------------------" << endl;
        
        // 测试所有访问模式
        vector<string> patterns = {
            "sequential", "circular", "random", 
            "zipf", "locality", "burst", "mixed"
        };
        
        // 创建结果文件
        ofstream result_file("results/lfu_" + to_string(capacity) + ".csv");
        result_file << "Pattern,HitRate,AvgAccessTime,MissCount" << endl;
        
        for (const string& pattern : patterns) {
            string pattern_file = test_data_dir + "/" + pattern + "_access.txt";
            PerformanceStats stats = test_pattern(pattern_file, capacity);
            
            // 输出结果
            cout << pattern << " 访问模式:" << endl;
            cout << "  命中率: " << fixed << setprecision(4) << stats.hit_rate() * 100 << "%" << endl;
            cout << "  平均访问时间: " << fixed << setprecision(6) << stats.average_access_time() * 1000000 << " μs" << endl;
            cout << "  未命中次数: " << stats.miss_count << endl;
            cout << endl;
            
            // 保存到CSV文件
            result_file << pattern << ","
                      << stats.hit_rate() << ","
                      << stats.average_access_time() << ","
                      << stats.miss_count << endl;
        }
        
        result_file.close();
    }
    
    cout << "\n测试完成！结果已保存到 results 目录" << endl;
    return 0;
}