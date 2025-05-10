// Link:
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* left, TreeNode* right)
        : val(x), left(left), right(right) {}
};
class Solution {
   public:
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        if (preorder.empty() || inorder.empty()) return nullptr;
        // 创建根节点
        TreeNode* root = new TreeNode(preorder[0]);

        // 在中序遍历中找到根节点的位置
        auto it = find(inorder.begin(), inorder.end(), preorder[0]);

        // 计算左子树的长度
        int leftSize = it - inorder.begin();

        // 分割前序遍历数组
        vector<int> leftPreorder(preorder.begin() + 1,
                                 preorder.begin() + 1 + leftSize);
        vector<int> rightPreorder(preorder.begin() + 1 + leftSize,
                                  preorder.end());

        // 分割中序遍历数组
        vector<int> leftInorder(inorder.begin(), it);
        vector<int> rightInorder(it + 1, inorder.end());

        // 递归构建左右子树
        root->left = buildTree(leftPreorder, leftInorder);
        root->right = buildTree(rightPreorder, rightInorder);

        return root;
    }
};
// 优化
class Solution1 {
   public:
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        // 创建哈希表存储中序遍历中每个值的位置
        unordered_map<int, int> inorderMap;
        for (int i = 0; i < inorder.size(); i++) {
            inorderMap[inorder[i]] = i;
        }

        // 使用索引来标记子数组的范围，避免创建新的vector
        return buildTreeHelper(preorder, inorder, 0, preorder.size() - 1, 0,
                               inorder.size() - 1, inorderMap);
    }

   private:
    TreeNode* buildTreeHelper(vector<int>& preorder, vector<int>& inorder,
                              int preStart, int preEnd, int inStart, int inEnd,
                              unordered_map<int, int>& inorderMap) {
        // 基本情况：如果范围无效，返回nullptr
        if (preStart > preEnd || inStart > inEnd) {
            return nullptr;
        }

        // 创建根节点
        TreeNode* root = new TreeNode(preorder[preStart]);

        // 获取根节点在中序遍历中的位置
        int rootIndex = inorderMap[preorder[preStart]];

        // 计算左子树的节点数量
        int leftSize = rootIndex - inStart;

        // 递归构建左右子树
        root->left = buildTreeHelper(preorder, inorder, preStart + 1,
                                     preStart + leftSize, inStart,
                                     rootIndex - 1, inorderMap);

        root->right =
            buildTreeHelper(preorder, inorder, preStart + leftSize + 1, preEnd,
                            rootIndex + 1, inEnd, inorderMap);

        return root;
    }
};
