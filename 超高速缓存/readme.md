# 缓存替换算法性能测试项目

本项目实现了多种缓存替换算法，并提供了完整的性能测试框架。通过模拟不同的访问模式，评估各种缓存算法在不同场景下的性能表现。

## 项目结构

```
.
├── LRU.cc          # LRU (Least Recently Used) 缓存实现
├── LFU.cc          # LFU (Least Frequently Used) 缓存实现
├── LRU_K.cc        # LRU-K 缓存实现
├── colck_pro.cc    # Clock-Pro 缓存实现
├── generate_test_data.py  # 测试数据生成脚本
├── test_all.py     # 所有算法对比测试脚本
├── test_data/      # 测试数据目录
└── results/        # 测试结果输出目录
```

## 实现的缓存算法

1. **LRU (Least Recently Used)**
   - 基于最近使用时间的缓存替换策略
   - 使用双向链表和哈希表实现，保证O(1)的访问复杂度

2. **LFU (Least Frequently Used)**
   - 基于访问频率的缓存替换策略
   - 优先淘汰访问次数最少的缓存项

3. **LRU-K**
   - LRU的改进版本，考虑最近K次访问的时间
   - 能够更好地处理突发访问模式

4. **Clock-Pro**
   - 基于Clock算法的改进版本
   - 使用引用位和修改位来优化缓存替换决策

## 测试数据生成

项目提供了多种访问模式的测试数据生成器：

- 顺序访问 (Sequential)
- 循环访问 (Circular)
- 随机访问 (Random)
- Zipf分布访问 (Zipf)
- 局部性访问 (Locality)
- 突发访问 (Burst)
- 混合访问 (Mixed)

## 编译和运行

### 环境要求

- C++11 或更高版本
- Python 3.6+
- 依赖包：numpy

### 安装依赖

```bash
pip install -r requirements.txt
```

### 生成测试数据

```bash
python generate_test_data.py
```

### 编译C++代码

```bash
# 编译单个算法
g++ -std=c++11 LRU.cc -o lru
g++ -std=c++11 LFU.cc -o lfu
g++ -std=c++11 LRU_K.cc -o lru_k
g++ -std=c++11 clock_pro.cc -o clock_pro
```

### 运行测试

```bash
./lru
./lruk
./lfu
./clock
```

## 测试结果

测试结果将保存在 `results` 目录下，包括：
- 命中率 (Hit Rate)
- 平均访问时间 (Average Access Time)
- 未命中次数 (Miss Count)

每种算法都会生成对应的CSV文件，方便后续分析和比较。

## 注意事项

1. 确保在运行测试前已生成测试数据
2. 测试数据默认生成在 `test_data` 目录下
3. 测试结果默认保存在 `results` 目录下
4. 可以根据需要修改 `generate_test_data.py` 中的参数来生成不同的测试数据
