import os
import shutil
from datetime import datetime

# 源文件夹路径
src_folder = r'D:\my_study_program\code\algorithm'

# 遍历所有文件
for filename in os.listdir(src_folder):
    file_path = os.path.join(src_folder, filename)
    if os.path.isfile(file_path):
        # 获取文件的修改时间
        t = os.path.getmtime(file_path)
        dt = datetime.fromtimestamp(t)
        folder_name = dt.strftime('%Y-%m-%d')  # 按年月日分
        target_folder = os.path.join(src_folder, folder_name)
        os.makedirs(target_folder, exist_ok=True)
        shutil.move(file_path, os.path.join(target_folder, filename))
