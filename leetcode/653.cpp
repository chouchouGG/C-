#include <stack>

using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
};


class Solution {
public:
    void update_l_Ptr(stack<TreeNode *> & s, TreeNode * &l) {
        if (l != nullptr) {
            s.push(l);
            l = l->left;
        }
        l = s.top();
    }
    
    void update_r_Ptr(stack<TreeNode *> & s, TreeNode * &r) {
        if (r != nullptr) {
            s.push(r);
            r = r->right;
        }
        r = s.top();
    }

    // 由于是BST，所以用两个指针分别指向最左端、最右端的节点，并逐渐相对而行
    bool findTarget(TreeNode* root, int k) {
        stack<TreeNode *> sl, sr; //两个栈
        TreeNode * l, * r;
        l = r = root;

        update_l_Ptr(sl, l); // l指向最左端
        update_r_Ptr(sr, r); // r指向最右端

        while (l != r) {
            int sum = l->val + r->val;
            if (sum == k) 
                return true;
            else if (sum < k) {
                l = l->right;
                sl.pop();
                update_l_Ptr(sl, l);
            }  
            else {
                r = r->left;
                sr.pop();
                update_r_Ptr(sr, r);
            }
        }
    }
};