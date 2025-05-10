// Link:
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
/**
 * Definition for a binary tree node.
 * */
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
    pair<TreeNode *, int> arr[3001];  // 存储节点和位置信息
   public:
    int widthOfBinaryTree(TreeNode *root) {
        if (!root) return 0;
        int l = 0, r = 0;
        arr[r++] = {root, 0};
        int maxWidth = 1;  // 至少有一个节点
        while (l < r) {
            int size = r - l;
            int leftPos = arr[l].second;       // 当前层最左边的位置
            int rightPos = arr[r - 1].second;  // 当前层最右边的位置
            maxWidth = maxWidth > (rightPos - leftPos + 1)
                           ? maxWidth
                           : (rightPos - leftPos + 1);
            while (size--) {
                TreeNode *curr = arr[l].first;
                if (curr && curr->left) {
                    arr[r++] = {curr->left, (arr[l].second << 1) +
                                                1};  // 使用父节点的位置计算
                }
                if (curr && curr->right) {
                    arr[r++] = {curr->right, (arr[l].second << 1) +
                                                 2};  // 使用父节点的位置计算
                }
                l++;
            }
        }
        return maxWidth;
    }
};
