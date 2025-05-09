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
    TreeNode *arr[2001];
    vector<vector<int>> ans;

   public:
    vector<vector<int>> levelOrder(TreeNode *root) {
        if (!root) return {};
        int l = 0, r = 0;
        arr[r++] = root;
        while (l < r) {
            int size = r - l;
            vector<int> vec;
            while (size--) {
                TreeNode *curr = arr[l++];
                vec.push_back(curr->val);
                if (curr->left) {
                    arr[r++] = curr->left;
                }
                if (curr->right) {
                    arr[r++] = curr->right;
                }
            }
            ans.push_back(vec);
        }
        return ans;
    }
};
