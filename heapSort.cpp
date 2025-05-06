// https://www.luogu.com.cn/problem/P1177
#include <iostream>
using namespace std;

// static long long nums[100001];

// void swap(long long &a, long long &b) {
//   long long temp = a;
//   a = b;
//   b = temp;
// }

// // 从i位置向下维护堆
// void heapify(int i, int size) {
//   int l = (i << 1) + 1; // 左子节点
//   while (l < size) {
//     // 选择左右子节点中较大的一个
//     int largest = l + 1 < size && nums[l + 1] > nums[l] ? l + 1 : l;
//     // 如果父节点已经大于等于最大子节点，停止调整
//     if (nums[i] >= nums[largest])
//       break;

//     // 交换父节点和最大子节点
//     swap(nums[i], nums[largest]);
//     // 继续向下调整
//     i = largest;
//     l = (i << 1) + 1;
//   }
// }

// // 向上调整堆
// void heapifyInsert(int i) {
//   // 当节点大于其父节点时，交换并继续向上
//   while (i > 0 && nums[i] > nums[(i - 1) >> 1]) {
//     swap(nums[i], nums[(i - 1) >> 1]);
//     i = (i - 1) >> 1;
//   }
// }

// // 堆排序
// void heapSort(int n) {
//   // 构建大顶堆
//   for (int i = n / 2 - 1; i >= 0; i--) {
//     heapify(i, n);
//   }

//   // 一个个提取最大元素
//   for (int i = n - 1; i > 0; i--) {
//     swap(nums[0], nums[i]); // 将最大元素放到末尾
//     heapify(0, i);          // 调整剩余的堆
//   }
// }

// int main() {
//   int n;
//   cin >> n;

//   // 读取数据
//   for (int i = 0; i < n; i++) {
//     cin >> nums[i];
//   }

//   // 排序
//   heapSort(n);

//   // 输出排序结果
//   for (int i = 0; i < n; i++) {
//     cout << nums[i] << " ";
//   }

//   return 0;
// }

// https://leetcode.cn/problems/sort-an-array/

class Solution {
public:
  vector<int> sortArray(vector<int> &nums) {
    heapSort(nums, nums.size());
    return nums;
  }
  void swap(vector<int> &nums, int a, int b) {
    int temp = nums[a];
    nums[a] = nums[b];
    nums[b] = temp;
  }
  // 从i位置向下维护堆
  void heapify(vector<int> &nums, int i, int size) {
    int l = (i << 1) + 1; // 左子节点
    while (l < size) {
      // 选择左右子节点中较大的一个
      int largest = l + 1 < size && nums[l + 1] > nums[l] ? l + 1 : l;
      // 如果父节点已经大于等于最大子节点，停止调整
      if (nums[i] >= nums[largest])
        break;

      // 交换父节点和最大子节点
      swap(nums, i, largest);
      // 继续向下调整
      i = largest;
      l = (i << 1) + 1;
    }
  }
  // 堆排序
  void heapSort(vector<int> &nums, int n) {
    // 构建大顶堆
    for (int i = n / 2 - 1; i >= 0; i--) {
      heapify(nums, i, n);
    }

    // 一个个提取最大元素
    for (int i = n - 1; i > 0; i--) {
      swap(nums, 0, i);    // 将最大元素放到末尾
      heapify(nums, 0, i); // 调整剩余的堆
    }
  }
};