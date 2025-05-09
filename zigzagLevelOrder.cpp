
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
    vector<vector<int>> zigzagLevelOrder(TreeNode *root) {
        if (!root) return {};
        int l = 0, r = 0;
        arr[r++] = root;
        bool isReverse = false;
        while (l < r) {
            int size = r - l;
            vector<int>vec;
            // if(reverse){
            //     for(int i = size - 1;i>=0;i--){
            //         vec.push_back(arr[i+ l]->val);
            //     }
            //     reverse = false;
            // }else{
            //     for(int i = 0;i< size;i++){
            //         vec.push_back(arr[i+l]->val);
            //     }
            //     reverse = true;
            // }

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

            if(isReverse) {
                reverse(vec.begin(), vec.end());
                isReverse = false;
            }else{
                isReverse = true;
            }
            ans.push_back(vec);
        }
        return ans;
    }
};
