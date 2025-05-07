// filepath: ~/code/Algorithm/getIntersectionNode.cpp
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct ListNode {
  int val;
  ListNode *next;
  ListNode(int x) : val(x), next(NULL) {}
};
class Solution {
public:
  ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
    int diff = 0;
    ListNode *a = headA, *b = headB;
    while (a) {
      diff++;
      a = a->next;
    }
    while (b) {
      diff--;
      b = b->next;
    }
    if (a != b) {
      return nullptr;
    }
    if (diff >= 0) {
      a = headA;
      b = headB;
    } else {
      a = headB;
      b = headA;
    }
    diff = abs(diff);
    while (diff--) {
      a = a->next;
    }
    while (a != b) {
      a = a->next;
      b = b->next;
    }
    return a;
  }
};