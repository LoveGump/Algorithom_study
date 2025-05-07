// filepath: ~/code/Algorithm/temp.cpp
// link:https://leetcode.cn/problems/palindrome-linked-list/
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
  ListNode() : val(0), next(nullptr) {}
  ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
public:
  ListNode *reverseList(ListNode *head) {
    if (!head)
      return nullptr;
    ListNode *prev = nullptr, *cur = head, *next = head->next;
    while (cur) {
      next = cur->next;
      cur->next = prev;
      prev = cur;
      cur = next;
    }
    return prev;
  }
  bool isPalindrome(ListNode *head) {
    if (!head || !head->next)
      return true;
    ListNode *fast = head;
    ListNode *slow = head; // slow 最终位置靠右
    ListNode *prev = nullptr;
    int size = 1;
    while (fast && fast->next) {
      size++;
      prev = slow;
      slow = slow->next;
      fast = fast->next->next;
    }
    slow = reverseList(slow);
    fast = head;
    while (--size) {
      if (slow->val != fast->val) {
        prev->next = reverseList(prev->next);
        return false;
      }
      slow = slow->next;
      fast = fast->next;
    }
    prev->next = reverseList(prev->next);
    return true;
  }
};