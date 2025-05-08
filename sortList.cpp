
// filepath: ~/code/Algorithm/sortList.cpp
// link:https://leetcode.cn/problems/sort-list/
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
  ListNode *sortList(ListNode *head) {
    if (!head || !head->next) return head;

    // 计算链表长度
    int length = 0;
    ListNode *curr = head;
    while (curr) {
      length++;
      curr = curr->next;
    }

    ListNode dummy(0);
    dummy.next = head;

    // 自底向上归并，size 从 1 开始翻倍
    for (int size = 1; size < length; size <<= 1) {
      ListNode *prev = &dummy;
      curr = dummy.next;

      // 每次合并 size 大小的两个子链表
      while (curr) {
        // 准备左半部分
        ListNode *left = curr;
        ListNode *right = split(left, size);
        curr = split(right, size);  // 准备下一轮的起点

        // 合并左右两部分并连接到结果链表
        prev->next = merge(left, right);

        // 移动 prev 到合并后链表的末尾
        while (prev->next) {
          prev = prev->next;
        }
      }
    }

    return dummy.next;
  }

 private:
  // 分割链表，返回第 n 个节点后的部分，并切断连接
  ListNode *split(ListNode *head, int n) {
    if (!head) return nullptr;

    // 找到第 n-1 个节点
    for (int i = 1; i < n && head->next; i++) {
      head = head->next;
    }

    ListNode *rest = head->next;
    head->next = nullptr;  // 切断链接
    return rest;
  }

  // 合并两个有序链表
  ListNode *merge(ListNode *l1, ListNode *l2) {
    ListNode dummy(0);
    ListNode *tail = &dummy;

    while (l1 && l2) {
      if (l1->val <= l2->val) {
        tail->next = l1;
        l1 = l1->next;
      } else {
        tail->next = l2;
        l2 = l2->next;
      }
      tail = tail->next;
    }

    tail->next = l1 ? l1 : l2;

    return dummy.next;
  }
};
