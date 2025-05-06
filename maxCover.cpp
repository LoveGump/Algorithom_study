// https://www.nowcoder.com/practice/1ae8d0b6bb4e4bcdbf64ec491f63fc37/
#include <iostream>
#include <algorithm> // 添加算法头文件
#include <queue>
#include <vector>
using namespace std;

int main() {
    int n;
    cin >> n;
    vector<pair<int, int>> intervals(n);
    
    for(int i = 0; i < n; i++){
        cin >> intervals[i].first >> intervals[i].second;
    }
    
    // 根据第一个元素排序
    sort(intervals.begin(), intervals.end());
    
    int ans = 0;
    priority_queue<int,vector<int>,greater<int>> pq;
    for(int i = 0; i < n; i++){
        while(!pq.empty() && pq.top() <= intervals[i].first){
            pq.pop();
        }
        pq.push(intervals[i].second); // 应该是放入结束时间而不是开始时间
        
        ans = max(ans, (int)pq.size());
    }
    
    cout << ans;
    return 0;
}