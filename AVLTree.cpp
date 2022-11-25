#include "AVLTree.h"

//DON'T FORGET TO ADD D'CTOR

AVLTree::AVLTree(): root(nullptr) {}

int AVLTree::height(TreeNode* node) const {
    if(node == nullptr) {
        return 0;
    }
    return node->height;
}

int AVLTree::balanceFactor(TreeNode* node) const {
    if(node == nullptr) {
        return 0;
    }
    return AVLTree::height(node->left) - AVLTree::height(node->right);
}

TreeNode* AVLTree::RRRotation(TreeNode* root) {
    TreeNode* oldRoot = root;
    TreeNode* newRoot = root->right;
    oldRoot->right = newRoot->left;
    newRoot->left = oldRoot;
    return newRoot;
}

TreeNode* AVLTree::LLRotation(TreeNode* root) {
    TreeNode* oldRoot = root;
    TreeNode* newRoot = root->left;
    oldRoot->left = newRoot->right;
    newRoot->right = oldRoot;
    return newRoot;
}

TreeNode* AVLTree::LRRotation(TreeNode* root) {
    TreeNode* oldRoot = root;
    TreeNode* node = root->left;
    TreeNode* newRoot = root->left->right;
    oldRoot->left = newRoot->right;
    node->right = newRoot->left;
    newRoot->right = oldRoot;
    newRoot->left = node;
    return newRoot;
}

TreeNode* AVLTree::RLRotation(TreeNode* root) {
    TreeNode* oldRoot = root;
    TreeNode* node = root->right;
    TreeNode* newRoot = root->right->left;
    oldRoot->right = newRoot->left;
    node->left = newRoot->right;
    newRoot->left = oldRoot;
    newRoot->right = node;
    return newRoot;
}

TreeNode* AVLTree::insert(TreeNode* root, int val) {
    if(root == nullptr) {
        return new TreeNode(val);
    }
    if(val < root->val) {
        root->left = insert(root->left, val);
    }
    else if (val > root->val) {
        root->right = insert(root->right, val);
    }
    else { //same vals - illegal
        return root;
    }
    root->height = AVLTree::max(height(root->left), height(root->right)) + 1;
    int balance = AVLTree::balanceFactor(root);
    if(balance == 2) {
        if(AVLTree::balanceFactor(root->left) >= 0) { //Left Left Rotation
            return AVLTree::LLRotation(root);
        }
        else { //Left Right Rotation
            return AVLTree::LRRotation(root);
        }
    }
    if(balance == -2) {
        if(AVLTree::balanceFactor(root->right) <= 0) { //Right Right Rotation
            return AVLTree::RRRotation(root);
        }
        else { //Right Left Rotation
            return AVLTree::RLRotation(root);
        }
    }
    return root;
}