// filepath: ~/code/Algorithm/MedianFinder.cpp
// link:
#include <algorithm>
#include <iostream>
#include <list>
#include <string>
#include <vector>
using namespace std;

class MedianFinder {
   public:
    // 最大堆，存储较小的一半数字
    priority_queue<int> maxHeap;

    // 最小堆，存储较大的一半数字
    priority_queue<int, vector<int>, greater<int>> minHeap;
    MedianFinder() {}

    void addNum(int num) {
        // 如果maxHeap为空，或者num小于maxHeap的堆顶，放入maxHeap
        if (maxHeap.empty() || num <= maxHeap.top()) {
            maxHeap.push(num);
        } else {
            minHeap.push(num);
        }

        // 调整
        if (maxHeap.size() > minHeap.size() + 1) {
            minHeap.push(maxHeap.top());
            maxHeap.pop();
        }  // 保证
        else if (minHeap.size() > maxHeap.size()) {
            maxHeap.push(minHeap.top());
            minHeap.pop();
        }
    }

    double findMedian() {
        // 元素总数为奇数时，中位数是maxHeap的堆顶
        if (maxHeap.size() > minHeap.size()) {
            return maxHeap.top();
        }
        // 元素总数为偶数时，中位数是两个堆顶的平均值
        else {
            return (maxHeap.top() + minHeap.top()) / 2.0;
        }
    }
};

/**
 * Your MedianFinder object will be instantiated and called as such:
 * MedianFinder* obj = new MedianFinder();
 * obj->addNum(num);
 * double param_2 = obj->findMedian();
 */
