import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from matplotlib import font_manager
import platform
import argparse

# 解析命令行参数
parser = argparse.ArgumentParser(description='Generate cache performance comparison charts.')
parser.add_argument('--scale', type=int, default=1024, help='Data scale (default: 1024)')
args = parser.parse_args()

# 根据操作系统设置中文字体
if platform.system() == 'Darwin':  # macOS
    font_path = '/System/Library/Fonts/STHeiti Light.ttc'
    font_prop = font_manager.FontProperties(fname=font_path)
    plt.rcParams['font.family'] = font_prop.get_name()
else:  # Windows 或 Linux
    plt.rcParams['font.sans-serif'] = ['SimHei', 'Microsoft YaHei', 'Arial Unicode MS']
plt.rcParams['axes.unicode_minus'] = False

# 读取所有CSV文件
lfu_data = pd.read_csv(f'lfu_{args.scale}.csv')
lru_data = pd.read_csv(f'lru_{args.scale}.csv')
clockpro_data = pd.read_csv(f'clockpro_{args.scale}.csv')
lruk2_data = pd.read_csv(f'lruk_{args.scale}_2.csv')
lruk3_data = pd.read_csv(f'lruk_{args.scale}_3.csv')
lruk4_data = pd.read_csv(f'lruk_{args.scale}_4.csv')

# 设置图表风格
plt.style.use('seaborn-v0_8')
sns.set_theme()

# 创建图表（只要三张）
fig, (ax1, ax2, ax3) = plt.subplots(1, 3, figsize=(21, 6))
fig.suptitle(f'缓存算法性能比较 (数据规模: {args.scale})', fontsize=16, fontproperties=font_prop)

# 1. 命中率比较
algorithms = ['LFU', 'LRU', 'ClockPro', 'LRUK-2', 'LRUK-3', 'LRUK-4']
patterns = lfu_data['Pattern']
hit_rates = [lfu_data['HitRate'], lru_data['HitRate'], 
             clockpro_data['HitRate'], lruk2_data['HitRate'], 
             lruk3_data['HitRate'], lruk4_data['HitRate']]

x = range(len(patterns))
width = 0.12  # 调整宽度以适应更多的算法

for i, (algo, rates) in enumerate(zip(algorithms, hit_rates)):
    ax1.bar([j + i*width for j in x], rates, width, label=algo)

ax1.set_ylabel('命中率', fontproperties=font_prop)
ax1.set_title('不同访问模式下的命中率', fontproperties=font_prop)
ax1.set_xticks([i + width*2.5 for i in x])
ax1.set_xticklabels(patterns, rotation=45)
ax1.legend()

# 2. 平均访问时间比较
access_times = [lfu_data['AvgAccessTime'], lru_data['AvgAccessTime'],
                clockpro_data['AvgAccessTime'], lruk2_data['AvgAccessTime'],
                lruk3_data['AvgAccessTime'], lruk4_data['AvgAccessTime']]

for i, (algo, times) in enumerate(zip(algorithms, access_times)):
    ax2.bar([j + i*width for j in x], times, width, label=algo)

ax2.set_ylabel('平均访问时间 (秒)', fontproperties=font_prop)
ax2.set_title('不同访问模式下的平均访问时间', fontproperties=font_prop)
ax2.set_xticks([i + width*2.5 for i in x])
ax2.set_xticklabels(patterns, rotation=45)
ax2.legend()

# 3. 未命中次数比较
miss_counts = [lfu_data['MissCount'], lru_data['MissCount'],
               clockpro_data['MissCount'], lruk2_data['MissCount'],
               lruk3_data['MissCount'], lruk4_data['MissCount']]

for i, (algo, counts) in enumerate(zip(algorithms, miss_counts)):
    ax3.bar([j + i*width for j in x], counts, width, label=algo)

ax3.set_ylabel('未命中次数', fontproperties=font_prop)
ax3.set_title('不同访问模式下的未命中次数', fontproperties=font_prop)
ax3.set_xticks([i + width*2.5 for i in x])
ax3.set_xticklabels(patterns, rotation=45)
ax3.legend()

plt.tight_layout(rect=[0, 0, 1, 0.95])
plt.savefig(f'{args.scale}.png', dpi=300, bbox_inches='tight')
plt.close() 