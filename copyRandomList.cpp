// filepath: ~/code/Algorithm/copyRandomList.cpp
// link:https://leetcode.cn/problems/copy-list-with-random-pointer/
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Definition for a Node.
class Node {
   public:
    int val;
    Node *next;
    Node *random;

    Node(int _val) {
        val = _val;
        next = NULL;
        random = NULL;
    }
};

class Solution {
   public:
    Node *copyRandomList(Node *head) {
        if (!head) return nullptr;

        // 阶段1: 复制节点并插入到原始节点之后
        Node *curr = head;
        Node *copy;
        while (curr) {
            copy = new Node(curr->val);
            copy->next = curr->next;
            curr->next = copy;
            curr = copy->next;
        }

        // 阶段2: 设置随机指针
        curr = head;
        while (curr) {
            if (curr->random) {
                curr->next->random = curr->random->next;
            }
            curr = curr->next->next;
        }

        // 阶段3: 分离两个链表
        Node *newHead = head->next;
        curr = head;
        copy = newHead;

        while (curr) {
            curr->next = copy->next;
            curr = curr->next;

            if (curr) {
                copy->next = curr->next;
                copy = copy->next;
            }
        }

        return newHead;
    }
};
