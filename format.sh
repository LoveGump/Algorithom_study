#!/bin/bash

# 检查是否提供了文件名参数
if [ $# -eq 0 ]; then
    echo "Usage: $0 <filename>"
    exit 1
fi

# 获取文件名
filename=$1

# 检查文件是否存在
if [ ! -f "$filename" ]; then
    echo "Error: File '$filename' not found"
    exit 1
fi

# 使用clang-format格式化文件
echo "Formatting $filename..."
clang-format -i "$filename"

# 检查clang-format是否成功
if [ $? -ne 0 ]; then
    echo "Error: clang-format failed"
    exit 1
fi

# 提交到本地仓库
echo "Committing changes..."
git add "$filename"
git commit -m "Format $filename using clang-format"

echo "Done!"
