#include <iostream>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        if (!root || root == p || root == q) {
            return root;
        }
        TreeNode* curr = root;
        while (true) {
            if (p->val < curr->val && q->val < curr->val) {
                curr = curr->left;
            } else if (p->val > curr->val && q->val > curr->val) {
                curr = curr->right;
            } else {
                return curr;
            }
        }
    }
};

// 创建测试树
TreeNode* createTestTree() {
    // 创建如下BST:
    //       6
    //      / \
    //     2   8
    //    / \ / \
    //   0  4 7  9
    //     / \
    //    3   5

    TreeNode* root = new TreeNode(6);

    root->left = new TreeNode(2);
    root->right = new TreeNode(8);

    root->left->left = new TreeNode(0);
    root->left->right = new TreeNode(4);
    root->right->left = new TreeNode(7);
    root->right->right = new TreeNode(9);

    root->left->right->left = new TreeNode(3);
    root->left->right->right = new TreeNode(5);

    return root;
}

int main() {
    Solution solution;
    TreeNode* root = createTestTree();

    // 测试用例1: p=2, q=8
    TreeNode* p1 = root->left;  // 值为2
    TreeNode* q1 = root->right; // 值为8
    TreeNode* result1 = solution.lowestCommonAncestor(root, p1, q1);
    cout << "Test Case 1: LCA of 2 and 8 is " << result1->val << endl;  // 应该输出6

    // 测试用例2: p=0, q=5
    TreeNode* p2 = root->left->left;     // 值为0
    TreeNode* q2 = root->left->right->right; // 值为5
    TreeNode* result2 = solution.lowestCommonAncestor(root, p2, q2);
    cout << "Test Case 2: LCA of 0 and 5 is " << result2->val << endl;  // 应该输出2

    // 测试用例3: p=3, q=5
    TreeNode* p3 = root->left->right->left;  // 值为3
    TreeNode* q3 = root->left->right->right; // 值为5
    TreeNode* result3 = solution.lowestCommonAncestor(root, p3, q3);
    cout << "Test Case 3: LCA of 3 and 5 is " << result3->val << endl;  // 应该输出4

    return 0;
}
