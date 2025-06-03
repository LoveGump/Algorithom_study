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
            return ValueType();
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
    LRUCache<int, int> cache(capacity);
    
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
        ofstream result_file("results/lru_" + to_string(capacity) + ".csv");
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