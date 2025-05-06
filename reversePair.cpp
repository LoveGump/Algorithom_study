#include <iostream>
#include <vector>

using namespace std;
class Solution {
public:
  int reversePairs(vector<int> &nums) {
    int size = nums.size();
    int l = 0, r = size - 1, res = 0;
    vector<int> help(size);
    return counts(nums, help, l, r);
  }
  int merge(vector<int> &inArr, vector<int> &help, int l, int m, int r) {
    int ans = 0;
    for (int i = l, j = m + 1, sum = 0; i <= m; i++) {
      while (j <= r && inArr[i] > (long long)2 * inArr[j]) {

        j++;
      }
      ans += j - m - 1;
    }

    // 正常归并排序
    int i = l;
    int a = l;
    int b = m + 1;
    while (a <= m && b <= r) {
      help[i++] = inArr[a] < inArr[b] ? inArr[a++] : inArr[b++];
    }
    while (a <= m) {
      help[i++] = inArr[a++];
    }
    while (b <= r) {
      help[i++] = inArr[b++];
    }
    for (int i = l; i <= r; i++) {
      inArr[i] = help[i];
    }
    return ans;
  }
  int counts(vector<int> &nums, vector<int> &help, int l, int r) {
    if (l == r)
      return 0;
    int m = (l + r) >> 1;
    return counts(nums, help, l, m) + counts(nums, help, m + 1, r) +
           merge(nums, help, l, m, r);
  }
};
int main() {
  vector<int> nums = {2, 4, 3, 5, 1};
  Solution a;
  cout << a.reversePairs(nums);
}