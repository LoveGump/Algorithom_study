import numpy as np
import random
from typing import List, Tuple
import json
from pathlib import Path

class TestDataGenerator:
    def __init__(self, 
                 sequence_length: int = 100000,
                 data_space_size: int = 10000,
                 cache_sizes: List[int] = [16, 32, 64, 128, 256, 512, 1024, 2048, 4096]):
        """
        初始化测试数据生成器
        
        Args:
            sequence_length: 访问序列长度，默认100,000
            data_space_size: 数据空间大小，默认10,000
            cache_sizes: 缓存大小列表，默认从16到4096，按2的幂次递增
        """
        self.sequence_length = sequence_length
        self.data_space_size = data_space_size
        self.cache_sizes = cache_sizes
        
        # 验证参数
        if max(cache_sizes) >= data_space_size:
            raise ValueError("缓存大小不能大于或等于数据空间大小")
        if sequence_length < 1000:
            raise ValueError("访问序列长度至少需要1000次访问")
            
    def generate_sequential_access(self) -> List[int]:
        """生成顺序访问模式"""
        return list(range(1, self.sequence_length + 1))
    
    def generate_circular_access(self, cycle_length: int = 1000) -> List[int]:
        """生成循环访问模式"""
        return [i % cycle_length + 1 for i in range(self.sequence_length)]
    
    def generate_random_access(self) -> List[int]:
        """生成随机访问模式"""
        return [random.randint(1, self.data_space_size) for _ in range(self.sequence_length)]
    
    def generate_zipf_access(self, alpha: float = 1.0) -> List[int]:
        """生成Zipf分布访问模式"""
        # 生成Zipf分布的权重
        weights = np.array([1.0 / (i ** alpha) for i in range(1, self.data_space_size + 1)])
        weights = weights / np.sum(weights)
        
        # 根据权重生成访问序列
        return np.random.choice(
            range(1, self.data_space_size + 1),
            size=self.sequence_length,
            p=weights
        ).tolist()
    
    def generate_locality_access(self, 
                               locality_size: int = 100,
                               locality_duration: int = 1000) -> List[int]:
        """生成局部性访问模式"""
        sequence = []
        current_locality = 1
        
        while len(sequence) < self.sequence_length:
            # 在局部性范围内生成访问
            locality_items = list(range(
                current_locality,
                min(current_locality + locality_size, self.data_space_size + 1)
            ))
            
            # 生成当前局部性期间的访问
            for _ in range(locality_duration):
                if len(sequence) >= self.sequence_length:
                    break
                sequence.append(random.choice(locality_items))
            
            # 切换到新的局部性区域
            current_locality = (current_locality + locality_size) % self.data_space_size
            if current_locality == 0:
                current_locality = 1
                
        return sequence
    
    def generate_burst_access(self, 
                            burst_probability: float = 0.1,
                            burst_size: int = 100) -> List[int]:
        """生成突发访问模式"""
        sequence = []
        while len(sequence) < self.sequence_length:
            if random.random() < burst_probability:
                # 生成突发访问
                burst_items = random.sample(range(1, self.data_space_size + 1), burst_size)
                sequence.extend(burst_items)
            else:
                # 生成正常访问
                sequence.append(random.randint(1, self.data_space_size))
                
        return sequence[:self.sequence_length]
    
    def generate_mixed_access(self) -> List[int]:
        """生成混合访问模式"""
        # 将序列分成多个部分，每部分使用不同的访问模式
        part_length = self.sequence_length // 6
        sequence = []
        
        # 顺序访问
        sequence.extend(self.generate_sequential_access()[:part_length])
        # 循环访问
        sequence.extend(self.generate_circular_access()[:part_length])
        # 随机访问
        sequence.extend(self.generate_random_access()[:part_length])
        # Zipf访问
        sequence.extend(self.generate_zipf_access()[:part_length])
        # 局部性访问
        sequence.extend(self.generate_locality_access()[:part_length])
        # 突发访问
        sequence.extend(self.generate_burst_access()[:part_length])
        
        return sequence[:self.sequence_length]
    
    def generate_all_patterns(self) -> dict:
        """生成所有访问模式的数据"""
        patterns = {
            'sequential': self.generate_sequential_access(),
            'circular': self.generate_circular_access(),
            'random': self.generate_random_access(),
            'zipf': self.generate_zipf_access(),
            'locality': self.generate_locality_access(),
            'burst': self.generate_burst_access(),
            'mixed': self.generate_mixed_access()
        }
        return patterns
    
    def save_patterns(self, output_dir: str = 'test_data'):
        """保存所有访问模式数据到文件"""
        patterns = self.generate_all_patterns()
        output_path = Path(output_dir)
        output_path.mkdir(exist_ok=True)
        
        # 保存实验配置
        config = {
            'sequence_length': self.sequence_length,
            'data_space_size': self.data_space_size,
            'cache_sizes': self.cache_sizes,
            'patterns': list(patterns.keys())
        }
        
        with open(output_path / 'experiment_config.json', 'w') as f:
            json.dump(config, f, indent=2)
        
        for pattern_name, sequence in patterns.items():
            file_path = output_path / f'{pattern_name}_access.json'
            with open(file_path, 'w') as f:
                json.dump({
                    'pattern': pattern_name,
                    'sequence_length': len(sequence),
                    'data_space_size': self.data_space_size,
                    'cache_sizes': self.cache_sizes,
                    'sequence': sequence
                }, f, indent=2)
            
            # 同时保存为文本文件，方便查看
            txt_path = output_path / f'{pattern_name}_access.txt'
            with open(txt_path, 'w') as f:
                f.write('\n'.join(map(str, sequence)))

def main():
    # 创建数据生成器
    generator = TestDataGenerator()
    
    # 生成并保存所有访问模式数据
    generator.save_patterns()
    
    print("测试数据生成完成！")
    print("生成的数据保存在 test_data 目录下")
    print("每种访问模式都生成了 .json 和 .txt 两种格式的文件")
    print("\n实验配置：")
    print(f"- 访问序列长度：{generator.sequence_length}")
    print(f"- 数据空间大小：{generator.data_space_size}")
    print(f"- 缓存大小范围：{generator.cache_sizes}")

if __name__ == '__main__':
    main() 