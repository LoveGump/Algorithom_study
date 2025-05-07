#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

class Solution {
public:
  int halveArray(vector<int> &nums) {
    return solution(nums);

  }
  int solution(vector<int> &nums) {
    int n = nums.size();
    priority_queue<double> pq;
    double sum = 0;
    for (int i = 0; i < n; ++i) {
      pq.push(nums[i]);
      sum += nums[i];
    }
    double target = sum / 2;
    int ans = 0;
    while (sum > target) {
      double top = pq.top();
      pq.pop();
      sum -= top / 2;
      pq.push(top / 2);
      ++ans;
    }
    return ans;  
  }
};