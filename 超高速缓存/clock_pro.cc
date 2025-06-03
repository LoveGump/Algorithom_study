#include <iostream>
#include <unordered_map>
#include <list>
#include <cassert>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>

using namespace std;

// 页类型枚举
enum class PageType { HOT, COLD, TEST };

// 页结构体
template<typename KeyType, typename ValueType>
struct Page {
    KeyType key;
    ValueType value;
    PageType type;
    bool referenced; // 是否被访问过
    Page(const KeyType& k, const ValueType& v, PageType t)
        : key(k), value(v), type(t), referenced(true) {}
};

// 时钟页面置换算法
template<typename KeyType, typename ValueType>
class ClockProCache {
public:

    /**
     * @brief 构造函数，初始化缓存容量,热页最大数量，冷页最大数量
     * @param capacity 缓存的容量
     */
    ClockProCache(size_t capacity)
        : capacity_(capacity), hot_max_(capacity / 2), cold_max_(capacity - hot_max_) {}

    /**
     * @brief 或取value值
     */
    ValueType get(const KeyType& key) {
        auto it = page_table_.find(key);
        if (it == page_table_.end()) {
            miss_count_++;
            
            return ValueType();
        }
        auto page_it = it->second;
        page_it->referenced = true;
        if (page_it->type == PageType::COLD) {
            page_it->type = PageType::HOT;
            hot_size_++;
            cold_size_--;
            adjust_hot();
        }
        return page_it->value;
    }

    /**
     * 插入key-value
     */
    void put(const KeyType& key, const ValueType& value) {
        auto it = page_table_.find(key);
        if (it != page_table_.end()) {
            // 已存在，更新并标记
            auto page_it = it->second;
            page_it->value = value;
            page_it->referenced = true;
            if (page_it->type == PageType::COLD) {
                page_it->type = PageType::HOT;
                hot_size_++;
                cold_size_--;
                adjust_hot();
            }
            return;
        }
        miss_count_++;
        // 新插入
        if (hot_size_ + cold_size_ >= capacity_) {
            replace();
        }
        pages_.emplace_back(key, value, PageType::COLD);
        auto new_it = --pages_.end();
        page_table_[key] = new_it;
        cold_size_++;
    }

    void display() const {
        for (const auto& page : pages_) {
            cout << page.key << ":" << page.value
                      << (page.type == PageType::HOT ? "(H)" : "(C)")
                      << (page.referenced ? "*" : "") << " ";
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
     * @brief 调整热页数量
     */
    void adjust_hot() {
        // 保证热页数量不超过 hot_max_
        while (hot_size_ > hot_max_) {
            auto it = pages_.begin();
            // 找到第一个类型为 HOT（热页）的页面
            // 如果找到，则将其类型改为 COLD（冷页），并更新热页数量和冷页数量
            // 如果未找到，则遍历结束
            while (it != pages_.end()) {
                if (it->type == PageType::HOT) {
                    it->type = PageType::COLD;
                    hot_size_--;
                    cold_size_++;
                    break;
                }
                ++it;
            }
        }
    }

    void replace() {
        // CLOCK指针遍历，优先淘汰冷页
        auto it = pages_.begin(); 
        while (true) {
            if (it == pages_.end()) it = pages_.begin();  // 环形遍历
            if (it->type == PageType::COLD) {
                if (it->referenced) {
                    it->referenced = false;    // 如果被访问过，清除这个访问记录，"缓刑"
                } else {
                    page_table_.erase(it->key);
                    it = pages_.erase(it);
                    cold_size_--;
                    break;
                }
            }
            ++it;
        }
    }

    size_t capacity_;  // 缓存容量
    size_t hot_max_;  // 热页最大数量
    size_t cold_max_;  // 冷页最大数量
    size_t hot_size_ = 0, cold_size_ = 0;  // 热页数量，冷页数量
    list<Page<KeyType, ValueType>> pages_;  // 页列表，

    // 页表，key -> 页迭代器
    unordered_map<KeyType, typename list<Page<KeyType, ValueType>>::iterator> page_table_;
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
    ClockProCache<int, int> cache(capacity);
    
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
    stats.hit_count = 100000 - stats.miss_count; // 假设总访问次数为100000
    
    return stats;
}

int main() {
    // 测试参数
    const size_t TOTAL_DATA_SIZE = 100000;  // 总数据量
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
             << " (" << (capacity * 100.0 / TOTAL_DATA_SIZE) << "% of data space)" << endl;
        cout << "----------------------------------------" << endl;
        
        // 测试所有访问模式
        vector<string> patterns = {
            "sequential", "circular", "random", 
            "zipf", "locality", "burst", "mixed"
        };
        
        // 创建结果文件
        ofstream result_file("results/clockpro_" + to_string(capacity) + ".csv");
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