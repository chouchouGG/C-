// #include <bits/stdc++.h>

// using namespace std;

//     struct TreeNode 
//     {
//         int val;
//         TreeNode *left;
//         TreeNode *right;
//     };

//     vector<int> inorderTraversal(TreeNode* root) 
//     {
//         if (root == nullptr) return vector<int>(); // 判空检测
//         vector<int> ret;
//         stack<TreeNode*> stc;
//         stc.push(root); 

//         while (!stc.empty()) {
//             // 将当前节点的左子树全部压栈，直到没有
//             for (auto p = stc.top(); p->left != nullptr; p = p->left) {
//                 stc.push(p);
//             }
//             // 访问当前节点
//             auto curr = stc.top();
//             ret.push_back(curr->val);
//             stc.pop();
//             // 处理右子树
//             if (curr->right) stc.push(curr->right);
//         }
//         return ret;
//     }


// int main() 
// {

// }