import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from matplotlib import font_manager
import platform
import argparse

# 解析命令行参数
parser = argparse.ArgumentParser(description='Generate cache performance line charts based on different data scales.')
parser.add_argument('--scales', nargs='+', type=int, default=[1000, 1024], help='List of data scales (default: 1000 1024)')
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
data = {}
for scale in args.scales:
    data[scale] = {
        'lfu': pd.read_csv(f'lfu_{scale}.csv'),
        'lru': pd.read_csv(f'lru_{scale}.csv'),
        'clockpro': pd.read_csv(f'clockpro_{scale}.csv'),
        'lruk2': pd.read_csv(f'lruk_{scale}_2.csv'),
        'lruk3': pd.read_csv(f'lruk_{scale}_3.csv')
    }

# 设置图表风格
plt.style.use('seaborn-v0_8')
sns.set_theme()

# 创建图表
fig, (ax1, ax2, ax3) = plt.subplots(1, 3, figsize=(21, 6))
fig.suptitle('缓存算法性能比较 (不同数据规模)', fontsize=16, fontproperties=font_prop)

# 1. 命中率比较
algorithms = ['LFU', 'LRU', 'ClockPro', 'LRUK-2', 'LRUK-3']
patterns = data[args.scales[0]]['lfu']['Pattern']

for algo in algorithms:
    hit_rates = [data[scale][algo.lower()]['HitRate'].mean() for scale in args.scales]
    ax1.plot(args.scales, hit_rates, marker='o', label=algo)

ax1.set_xlabel('数据规模', fontproperties=font_prop)
ax1.set_ylabel('命中率', fontproperties=font_prop)
ax1.set_title('不同数据规模下的命中率', fontproperties=font_prop)
ax1.legend()

# 2. 平均访问时间比较
for algo in algorithms:
    access_times = [data[scale][algo.lower()]['AvgAccessTime'].mean() for scale in args.scales]
    ax2.plot(args.scales, access_times, marker='o', label=algo)

ax2.set_xlabel('数据规模', fontproperties=font_prop)
ax2.set_ylabel('平均访问时间 (秒)', fontproperties=font_prop)
ax2.set_title('不同数据规模下的平均访问时间', fontproperties=font_prop)
ax2.legend()

# 3. 未命中次数比较
for algo in algorithms:
    miss_counts = [data[scale][algo.lower()]['MissCount'].mean() for scale in args.scales]
    ax3.plot(args.scales, miss_counts, marker='o', label=algo)

ax3.set_xlabel('数据规模', fontproperties=font_prop)
ax3.set_ylabel('未命中次数', fontproperties=font_prop)
ax3.set_title('不同数据规模下的未命中次数', fontproperties=font_prop)
ax3.legend()

plt.tight_layout(rect=[0, 0, 1, 0.95])
plt.savefig('cache_performance_line_charts.png', dpi=300, bbox_inches='tight')
plt.close()
