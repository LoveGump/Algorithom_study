#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>
#include <map>
#include <unordered_map>
#include <deque>
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
PerformanceStats test_pattern(const string& pattern_file, size_t capacity, size_t k) {
    PerformanceStats stats;
    LRUKCache<int, int> cache(capacity, k);
    
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
    stats.hit_count = 100000 -  stats.miss_count; // 假设每次访问都要么命中要么未命中
    
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
    vector<size_t> k_values = {2, 3,4};
    string test_data_dir = "test_data";
    
    // 创建结果目录
    system("mkdir -p results");
    
    // 对每个缓存大小和K值进行测试
    for (size_t capacity : cache_sizes) {
        for (size_t k : k_values) {
            cout << "\n测试配置: 缓存大小=" << capacity << ", K=" << k << endl;
            cout << "----------------------------------------" << endl;
            
            // 测试所有访问模式
            vector<string> patterns = {
                "sequential", "circular", "random", 
                "zipf", "locality", "burst", "mixed"
            };
            
            // 创建结果文件
            ofstream result_file("results/lruk_" + to_string(capacity) + "_" + to_string(k) + ".csv");
            result_file << "Pattern,HitRate,AvgAccessTime,MissCount" << endl;
            
            for (const string& pattern : patterns) {
                string pattern_file = test_data_dir + "/" + pattern + "_access.txt";
                PerformanceStats stats = test_pattern(pattern_file, capacity, k);
                
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
    }
    
    cout << "\n测试完成！结果已保存到 results 目录" << endl;
    return 0;
}
