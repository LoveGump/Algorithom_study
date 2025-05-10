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
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};
class Codec {
   public:
    string serialize(TreeNode* root) {
        string str;
        preorder(root, str);
        return str;
    }

    void preorder(TreeNode* root, string& str) {
        if (!root) {
            str += "#,";
            return;
        }
        // 访问根节点
        str += to_string(root->val) + ",";

        // 遍历左子树
        preorder(root->left, str);

        // 遍历右子树
        preorder(root->right, str);
    }

    TreeNode* deserialize(string data) {
        if (data.empty()) return nullptr;

        // 将字符串按逗号分割
        vector<string> values;
        string value;
        for (char c : data) {
            if (c == ',') {
                if (!value.empty()) {  // 只添加非空值
                    values.push_back(value);
                    value.clear();
                }
            } else {
                value += c;
            }
        }
        int index = 0;
        return buildTree(values, index);
    }

    TreeNode* buildTree(vector<string>& values, int& index) {
        if (index >= values.size() || values[index] == "#") {
            index++;
            return nullptr;
        }

        TreeNode* root = new TreeNode(stoi(values[index++]));
        root->left = buildTree(values, index);
        root->right = buildTree(values, index);

        return root;
    }
};
