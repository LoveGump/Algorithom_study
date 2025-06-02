// filepath: ~/code/Algorithm/detectCycle.cpp
// link:https://leetcode.cn/problems/linked-list-cycle-ii/submissions/628156447/
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

/**
 * Definition for singly-linked list.
 * */
struct ListNode {
  int val;
  ListNode *next;
  ListNode(int x) : val(x), next(NULL) {}
};

class Solution {
public:
  ListNode *detectCycle(ListNode *head) {
    if (!head || !head->next)
      return nullptr;
    if (head->next == head)
      return head;
    ListNode *slow = head, *fast = head;
    while (fast && fast->next) {
      slow = slow->next;
      fast = fast->next->next;
      if (slow == fast) {
        slow = head;
        while (slow != fast) {
          slow = slow->next;
          fast = fast->next;
        }
        return slow;
      }
    }
    return nullptr;
  }
};
