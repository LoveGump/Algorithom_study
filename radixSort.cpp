#include <iostream>
#include <vector>
#include <cstring> // 用于memset
#include <algorithm> // 用于max_element
using namespace std;

void swap(int &a, int &b) {
  int temp = a;
  a = b;
  b = temp;
}

class Solution {
public:
    static const int BASE = 1000;
    static const int MAXN = 50001;
    
    vector<int> sortArray(vector<int> &nums) {
        if (nums.size() <= 1) return nums;
        
        // 处理负数：找到最小值
        int minValue = *min_element(nums.begin(), nums.end());
        
        // 如果有负数，将所有数字转为非负数
        if (minValue < 0) {
            for (int &num : nums) {
                num -= minValue;  // 平移，使最小值变为0
            }
        }
        
        // 找到最大值，确定位数
        int maxValue = *max_element(nums.begin(), nums.end());
        int bits = 0;
        while (maxValue > 0) {
            bits++;
            maxValue /= BASE;
        }
        
        // 执行基数排序
        radixSort(nums, nums.size(), bits);
        
        // 还原平移的数字
        if (minValue < 0) {
            for (int &num : nums) {
                num += minValue;  // 恢复原始值
            }
        }
        
        return nums;
    }
    
    void radixSort(vector<int> &nums, int n, int bits) {
        vector<int> temp(n);      // 临时数组
        vector<int> count(BASE);  // 计数数组
        
        for (int offset = 1; bits > 0; offset *= BASE, bits--) {
            // 重置计数数组
            memset(&count[0], 0, sizeof(int) * BASE);
            
            // 统计每个桶中的元素个数
            for (int i = 0; i < n; i++) {
                count[(nums[i] / offset) % BASE]++;
            }
            
            // 累加计数，计算每个数字的结束位置
            for (int i = 1; i < BASE; i++) {
                count[i] += count[i - 1];
            }
            
            // 从后往前遍历，保持稳定性
            for (int i = n - 1; i >= 0; i--) {
                int digit = (nums[i] / offset) % BASE;
                temp[count[digit] - 1] = nums[i];
                count[digit]--;
            }
            
            // 将临时数组的值复制回原数组
            for (int i = 0; i < n; i++) {
                nums[i] = temp[i];
            }
        }
    }
};