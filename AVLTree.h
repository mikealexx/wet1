#ifndef AVLTree_h
#define AVLTree_h

#include "TreeNode.h"

class AVLTree {
    private:
        static int max(int a, int b) {
            return (a>b) ? a : b;
        }
        int height(TreeNode* node) const;
        int balanceFactor(TreeNode* node) const;
        TreeNode* RRRotation(TreeNode* root);
        TreeNode* LLRotation(TreeNode* root);
        TreeNode* LRRotation(TreeNode* root);
        TreeNode* RLRotation(TreeNode* root);
    public:
        TreeNode* root;
        AVLTree();
        ~AVLTree() = default;
        TreeNode* insert(TreeNode* root, int val);
};

#endif