// Link:
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right)
        : val(x), left(left), right(right) {}
};
class Solution {
   public:
    bool isCompleteTree(TreeNode *root) {
        if (!root) return true;

        queue<TreeNode *> q;
        q.push(root);
        bool foundNull = false;  // 标记是否已经遇到空节点

        while (!q.empty()) {
            TreeNode *curr = q.front();
            q.pop();

            if (!curr) {
                foundNull = true;  // 遇到空节点
            } else {
                // 如果之前已经遇到空节点，但当前节点不是空，说明不是完全二叉树
                if (foundNull) return false;

                // 将左右子节点加入队列（即使是null也要加入）
                q.push(curr->left);
                q.push(curr->right);
            }
        }

        return true;
    }
};
