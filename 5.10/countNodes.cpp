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
    int countNodes(TreeNode *root) {
        if (!root) return 0;

        int l = getLength(root->left);
        int r = getLength(root->right);

        return l == r ? (1 << l) + countNodes(root->right)
                      : (1 << r) + countNodes(root->left);
    }
    int getLength(TreeNode *root) {
        if (!root) return 0;
        if (!root->left && !root->right) return 1;
        TreeNode *curr = root;
        int length = 1;
        while (curr->left) {
            length++;
            curr = curr->left;
        }
        return length;
    }
};
