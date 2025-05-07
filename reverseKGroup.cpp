// filepath: ~/code/Algorithm/reverseKGroup.cpp
// link:https://leetcode.cn/problems/reverse-nodes-in-k-group/description/
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

/**
 * Definition for singly-linked list.
 */

struct ListNode {
  int val;
  ListNode *next;
  ListNode() : val(0), next(nullptr) {}
  ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode *next) : val(x), next(next) {}
};
class Solution {
public:
  ListNode *reverseKGroup(ListNode *head, int k) {
    if (!head || k == 1)
      return head;
    // 创建哑节点
    ListNode dummy(0);
    dummy.next = head;

    // 初始化指针
    ListNode *prev = &dummy;
    ListNode *curr = head;
    ListNode *next = nullptr;
    // 统计链表长度
    int length = 0;
    while (head) {
      head = head->next;
      length++;
    }
    // 按k反转
    while (length >= k) {

      curr = prev->next; // 当前组的第一个节点

      next =
          curr->next; // 当前组的第二个节点
                      // 反转k-1次，将k个节点反转,每一次反转都要更新curr和next
      for (int i = 1; i < k; i++) {
        curr->next = next->next;
        next->next = prev->next;
        prev->next = next;
        next = curr->next;
      }
      prev = curr; // 更新prev到下一组的前一个节点
      length -= k; // 更新剩余节点数
    }
    return dummy.next;
  }
};