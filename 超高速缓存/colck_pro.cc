#include <iostream>
#include <unordered_map>
#include <list>
#include <cassert>
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
            cout << "key not found!!!" << endl;
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

int main() {
    size_t capacity;
    cout << "请输入缓存容量: ";
    cin >> capacity;
    ClockProCache<int, int> cache(capacity);
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