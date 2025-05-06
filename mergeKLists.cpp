// https://leetcode.cn/problems/merge-k-sorted-lists/submissions/627837761/

#include <iostream>
#include <vector>
#include <queue>
using namespace std;
// Definition for singly-linked list.
struct ListNode {
  int val;
  ListNode *next;
  ListNode() : val(0), next(nullptr) {}
  ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
public:
  ListNode *mergeKLists(vector<ListNode *> &lists) {
    // 建立小根堆
    auto cmp = [](ListNode *a ,ListNode*b){
        return a->val > b->val;
    } ;
    priority_queue<ListNode*, vector<ListNode*>, decltype(cmp)> pq(cmp);
    for(auto head:lists){
        if(head) pq.push(head); // 只将非空的链表头加入优先队列
    }
    if(pq.empty()){
        return nullptr;
    }
    // 创建哑节点和尾指针
    ListNode dummy(0);
    ListNode* tail = &dummy;
    // 不断从堆中取出最小节点，并将其下一个节点加入堆
    while (!pq.empty()) {
        ListNode* curr = pq.top();
        pq.pop();
        
        tail->next = curr;
        tail = tail->next;
        
        if (curr->next) pq.push(curr->next);
    }
    
    return dummy.next;
  }
};