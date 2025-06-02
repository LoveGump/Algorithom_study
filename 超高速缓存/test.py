import random
import numpy as np
import os

def generate_uniform_sequence(length, page_range):
    return [random.randint(1, page_range) for _ in range(length)]

def generate_zipf_sequence(length, page_range, alpha=1.2):
    sequence = []
    while len(sequence) < length:
        value = np.random.zipf(alpha)
        if value <= page_range:
            sequence.append(value)
    return sequence

def generate_locality_sequence(length, window_size, total_pages):
    sequence = []
    for _ in range(length // window_size):
        base = random.randint(1, total_pages - window_size)
        sequence.extend([random.randint(base, base + window_size) for _ in range(window_size)])
    return sequence

os.makedirs("test", exist_ok=True)

sizes = [50, 100, 200, 500, 1000]
page_range = 100  # 页面总数

for size in sizes:
    # 1. 均匀分布
    uniform_seq = generate_uniform_sequence(size, page_range)
    with open(f"test/{size}_uniform.in", "w") as f:
        for x in uniform_seq:
            f.write(f"get {x}\n")

    # 2. Zipf 分布
    zipf_seq = generate_zipf_sequence(size, page_range, alpha=1.3)
    with open(f"test/{size}_zipf.in", "w") as f:
        for x in zipf_seq:
            f.write(f"get {x}\n")

    # 3. 局部性模拟
    locality_seq = generate_locality_sequence(size, window_size=10, total_pages=page_range)
    with open(f"test/{size}_locality.in", "w") as f:
        for x in locality_seq:
            f.write(f"get {x}\n")

print("✅ 操作序列已生成并保存在 test 目录下！")