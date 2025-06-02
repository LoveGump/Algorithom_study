import random
import numpy as np
import os
from datetime import datetime

def zipf_distribution(n, alpha=1.0):
    """
    生成Zipf分布的概率
    n: 元素数量
    alpha: 分布参数，值越大分布越不均匀
    """
    x = np.arange(1, n + 1)
    weights = 1 / (x ** alpha)
    weights = weights / np.sum(weights)
    return weights

def generate_test_data(num_operations, max_key, max_value, alpha=1.0):
    # 生成操作序列
    operations = []
    for _ in range(num_operations):
        # 使用Zipf分布选择key
        key = np.random.choice(max_key, p=zipf_probs) + 1
        value = random.randint(1, max_value)
        operations.append(f"put {key} {value}")
    
    return operations

def save_test_data(operations, filename):
    with open(filename, 'w') as f:
        # 写入操作序列
        for op in operations:
            f.write(f"{op}\n")
        f.write("end\n")

if __name__ == "__main__":
    # 测试数据规模列表
    SCALES = [50, 100, 200, 500, 1000]
    
    # 其他参数
    MAX_KEY = 20          # 最大键值
    MAX_VALUE = 100       # 最大值
    ALPHA = 0.5           # Zipf分布参数，调整为1.2更符合内存访问规律
    
    # 为每个规模生成测试数据
    for num_operations in SCALES:
        # 生成Zipf分布的概率
        zipf_probs = zipf_distribution(MAX_KEY, ALPHA)
        
        # 生成测试数据
        operations = generate_test_data(num_operations, MAX_KEY, MAX_VALUE, ALPHA)
        
        # 生成文件名
        filename = os.path.join("test", f"{num_operations}.in")
        
        # 保存测试数据
        save_test_data(operations, filename)
        print(f"测试数据已保存到: {filename}") 