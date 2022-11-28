#ifndef AVLTree_h
#define AVLTree_h

#include "TreeNode.h"

template<class T, class S>
class AVLTree {
    private:
        static int max(int a, int b) {
            return (a>b) ? a : b;
        }

        static int height(const TreeNode<T>* node) {
            if(node == nullptr) {
                return 0;
            }
            return node->height;
        }

        static int balanceFactor(const TreeNode<T>* node) {
            if(node == nullptr) {
                return 0;
            }
            return AVLTree::height(node->left) - AVLTree::height(node->right);
        }

        static void destruct(const TreeNode<T>* node){
            if(node == nullptr){
                return;
            }
            if(node->left == nullptr && node->right == nullptr){
                delete(node);
                return;
            }
            destruct(node->left);
            destruct(node->right);
            destruct(node);
        }

        TreeNode<T>* minNode(TreeNode<T>* node) {
            TreeNode<T>* current = node;
            while (current->left != nullptr)
                current = current->left;
            return current;
        }

        TreeNode<T>* maxNode(TreeNode<T>* node) {
            TreeNode<T>* current = node;
            while (current->right != nullptr)
                current = current->right;
            return current;
        }

        TreeNode<T>* RRRotation(TreeNode<T>* root) {
            TreeNode<T>* oldRoot = root;
            TreeNode<T>* newRoot = root->right;
            oldRoot->right = newRoot->left;
            newRoot->left = oldRoot;
            return newRoot;
        }

        TreeNode<T>* LLRotation(TreeNode<T>* root) {
            TreeNode<T>* oldRoot = root;
            TreeNode<T>* newRoot = root->left;
            oldRoot->left = newRoot->right;
            newRoot->right = oldRoot;
            return newRoot;
        }

        TreeNode<T>* LRRotation(TreeNode<T>* root) {
            TreeNode<T>* oldRoot = root;
            TreeNode<T>* node = root->left;
            TreeNode<T>* newRoot = root->left->right;
            oldRoot->left = newRoot->right;
            node->right = newRoot->left;
            newRoot->right = oldRoot;
            newRoot->left = node;
            return newRoot;
        }

        TreeNode<T>* RLRotation(TreeNode<T>* root) {
            TreeNode<T>* oldRoot = root;
            TreeNode<T>* node = root->right;
            TreeNode<T>* newRoot = root->right->left;
            oldRoot->right = newRoot->left;
            node->left = newRoot->right;
            newRoot->left = oldRoot;
            newRoot->right = node;
            return newRoot;
        }

    public:
        TreeNode<T>* root;

        AVLTree();
        ~AVLTree();
        TreeNode<T>* insert(TreeNode<T>* root, T* data, const S& key);
        TreeNode<T>* remove(TreeNode<T>* root, const S& key);

        class KeyAlreadyExists : public std::exception {};
};

template<class T, class S>
AVLTree<T, S>::AVLTree(): root(nullptr) {}

template<class T, class S>
AVLTree<T, S>::~AVLTree() {
    destruct(this->root);
}



template<class T, class S>
TreeNode<T>* AVLTree<T, S>::insert(TreeNode<T>* root, T* data, const S& key) {
    if(root == nullptr) {
        return new TreeNode<T>(key, shared_ptr<T>(data));
    }
    if(key < root->key) { //locate correct insertion position
        root->left = insert(root->left, data, key);
    }
    else if (key > root->key) {
        root->right = insert(root->right, data, key);
    }
    else { //same keys - illegal
        throw AVLTree::KeyAlreadyExists();
    }
    root->height = AVLTree::max(height(root->left), height(root->right)) + 1;
    int balance = AVLTree::balanceFactor(root);
    if(balance == 2) {
        if(AVLTree::balanceFactor(root->left) >= 0) { //Left Left Rotation
            return AVLTree::LLRotation(root);
        }
        else { //left right rotation
            return AVLTree::LRRotation(root);
        }
    }
    if(balance == -2) {
        if(AVLTree::balanceFactor(root->right) <= 0) { //right right rotation
            return AVLTree::RRRotation(root);
        }
        else { //right left rotation
            return AVLTree::RLRotation(root);
        }
    }
    return root;
}

template<class T, class S>
TreeNode<T>* AVLTree<T, S>::remove(TreeNode<T>* root, const S& key) {
    if(root->left == nullptr && root->right == nullptr){
        if(root == this->root) {
            this->root == nullptr;
        }
        delete(root);
        return nullptr;
    }
    TreeNode<T>* temp;
    if(key > root->key) {
        root->right = remove(root->right, key);
    }
    else if(key < root->key) {
        root->left = remove(root->left, key);
    }
    else {
        if(root->left != nullptr) {
            temp = minNode(root->left);
            root->key = temp->key; //DONT FORGET TO ADD DATA
            root->left = remove(root->left, temp->key);
        }
        else {
            temp = maxNode(root->right);
            root->key = temp->key; //DONT FORGET TO ADD DATA
            root->right = remove(root->right, temp->key);
        }
    }
    if(balanceFactor(root) == 2 && balanceFactor(root->left) == 1) { //left left rotation
        root = LLRotation(root);
    }
    else if(balanceFactor(root) == 2 && balanceFactor(root->left) == -1) { //left right rotation
        root = LRRotation(root);
    }
    else if(balanceFactor(root) == 2 && balanceFactor(root->left) == 0) { //left left rotation
        root = LLRotation(root);
    }
    else if(balanceFactor(root) == -2 && balanceFactor(root->right) == -1) { //right right rotation
        root = RRRotation(root);
    }
    else if(balanceFactor(root) == -2 && balanceFactor(root->right) == 1) { //right left rotation
        root = RLRotation(root);
    }
    else if(balanceFactor(root) == -2 && balanceFactor(root->right) == 0){
        root = LLRotation(root);
    }
    return root;
}

#endif

//AVLTree<Player> playerTree;
//playerTree.insert(playerTree.root, )