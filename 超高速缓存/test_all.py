import os
import subprocess
import re

# 源文件和可执行文件名
programs = [
    ("LRU.cc", "LRU.exe"),
    ("LRU_K.cc", "LRU_K.exe"),
    ("LFU.cc", "LFU.exe"),
    ("colck_pro.cc", "colck_pro.exe"),
]

# 目录设置
test_dir = "test"
result_dir = "result"

# 确保结果目录存在
os.makedirs(result_dir, exist_ok=True)

# 测试参数
cache_size = 10  # 固定缓存容量
k_values = [2, 3, 4]      # 不同的K值

# # 编译所有程序
# for src, exe in programs:
#     print(f"Compiling {src} ...")
#     ret = subprocess.run(["g++", "-std=c++11", "-O2", src, "-o", exe])
#     if ret.returncode != 0:
#         print(f"Failed to compile {src}")
#         exit(1)

# 依次测试
for test_file in sorted(os.listdir(test_dir)):
    if not test_file.endswith(".in"):
        continue
    
    # 解析测试文件名
    size = test_file.split(".")[0]
    print(f"\nTesting {test_file} (size={size})")
    
    for src, exe in programs:
        if exe == "LRU_K.exe":
            # 对LRU-K测试不同的K值
            for k in k_values:
                result_path = os.path.join(result_dir, f"{exe}_{test_file}_k{k}.out")
                print(f"  Running {exe} with k={k}...")
                
                with open(os.path.join(test_dir, test_file), "r", encoding="utf-8") as fin, \
                     open(result_path, "w", encoding="utf-8") as fout:
                    pre_input = f"{cache_size}\n{k}\n"
                    test_data = fin.read()
                    full_input = pre_input + test_data
                    
                    proc = subprocess.run(
                        [exe],
                        input=full_input.encode("utf-8"),
                        stdout=fout,
                        stderr=subprocess.STDOUT,
                        timeout=30
                    )
        else:
            # 对其他算法测试
            result_path = os.path.join(result_dir, f"{exe}_{test_file}.out")
            print(f"  Running {exe}...")
            
            with open(os.path.join(test_dir, test_file), "r") as fin, \
                 open(result_path, "w") as fout:
                pre_input = f"{cache_size}\n"
                test_data = fin.read()
                full_input = pre_input + test_data
                
                proc = subprocess.run(
                    [exe],
                    input=full_input.encode("utf-8"),
                    stdout=fout,
                    stderr=subprocess.STDOUT,
                    timeout=30
                )

print("\nAll tests completed. Check the results in the result directory.")
