import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

name = 'clockpro'
# 读取所有CSV文件
cache_sizes = [16,64,256,1024,4096]
files = [f'{name}_{size}.csv' for size in cache_sizes]
dfs = [pd.read_csv(file) for file in files]

# 设置中文字体
plt.rcParams['font.sans-serif'] = ['Arial Unicode MS']  # 对于macOS
plt.rcParams['axes.unicode_minus'] = False

# 创建图表 - 改为横向排列
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(20, 6))

# 提取每种模式的命中率数据
patterns = dfs[0]['Pattern'].unique()
for pattern in patterns:
    hit_rates = [df[df['Pattern'] == pattern]['HitRate'].values[0] for df in dfs]
    ax1.plot(cache_sizes, hit_rates, marker='o', label=pattern)

ax1.set_xscale('log')
ax1.set_xlabel('缓存大小')
ax1.set_ylabel('命中率')
ax1.set_title('不同缓存大小下的命中率变化')
ax1.grid(True)
ax1.legend()

# 提取每种模式的平均访问时间数据
for pattern in patterns:
    access_times = [df[df['Pattern'] == pattern]['AvgAccessTime'].values[0] for df in dfs]
    ax2.plot(cache_sizes, access_times, marker='o', label=pattern)

ax2.set_xscale('log')
ax2.set_xlabel('缓存大小')
ax2.set_ylabel('平均访问时间 (秒)')
ax2.set_title('不同缓存大小下的平均访问时间变化')
ax2.grid(True)
ax2.legend()

plt.tight_layout()
plt.savefig(f'{name}.png', dpi=300, bbox_inches='tight')
plt.close() 