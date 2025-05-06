#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
  vector<int> sortArray(vector<int> &nums) {
    quickSort(nums, 0, nums.size() - 1);
    return nums;
  }
  void swap(vector<int> &nums, int a, int b) {
    int temp = nums[a];
    nums[a] = nums[b];
    nums[b] = temp;
  }
  void partition(vector<int> &nums, int l, int r, int x, int &a, int &b) {
    // 将数组分为  <x |  == x |  >x
    a = l, b = r;
    int i = l;
    while (b >= i) {
      if (nums[i] < x) {
        swap(nums, a, i);
        a++, i++;
      } else if (nums[i] == x) {
        i++;
      } else {
        swap(nums, i, b);
        b--;
      }
    }
  }
  void quickSort(vector<int> &nums, int l, int r) {
    if (l >= r)
      return;
    // 随机选择基准值以提高性能（避免最坏情况）
    int randomIndex = l + rand() % (r - l + 1);
    swap(nums, randomIndex, l);

    int x = nums[l];
    int newl = l, newr = r;
    partition(nums, l, r, x, newl, newr);
    quickSort(nums, l, newl - 1);
    quickSort(nums, newr + 1, r);
  }
};
int main() {
  vector<int> nums = {2, 4, 3, 5, 1};
  Solution a;
  a.sortArray(nums);
  for (int t : nums) {
    cout << t << " ";
  }
}