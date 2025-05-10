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
    int minDepth(TreeNode *root) {
        if (!root) return 0;
        if (!root->left && !root->right) return 1;

        int minl = root->left ? minDepth(root->left) : 10001;
        int minr = root->right ? minDepth(root->right) : 10001;

        return min(minl, minr) + 1;
    }
};
