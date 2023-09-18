#include <bits/stdc++.h>

using namespace std;

class Node {
public:
    int val;
    vector<Node*> children;
};

class Solution {
public:
    // 迭代法
    vector<int> preorder(Node* root) {
        vector<int> ret;
        stack<Node*> stk;
        stk.push(root);
        while (!stk.empty()) {
            auto & temp = stk.top();
            stk.pop();
            ret.push_back(temp->val);
            auto vec = temp->children;
            for (auto it = vec.rbegin(); it != vec.rend(); ++it) {
                stk.push(*it);
            }
        }
        return ret;
    }
};