#ifndef AVLTree_h
#define AVLTree_h

#include "TreeNode.h"

template<class T, class S>
class AVLTree {
    private:
        static int max(int a, int b) {
            return (a>b) ? a : b;
        }

        static int height(const TreeNode<T, S>* node) {
            if(node == nullptr) {
                return 0;
            }
            if (node->left == nullptr){
                if(node->right == nullptr) 
                    return 1;
                return node->right->height + 1;
            }
            if (node->right == nullptr)
                return node->left->height + 1;

            return max(node->left->height, node->right->height) + 1;
        }

        static int balanceFactor(const TreeNode<T, S>* node) {
            if(node == nullptr) {
                return 0;
            }
            return AVLTree::height(node->left) - AVLTree::height(node->right);
        }

        static void destruct(const TreeNode<T, S>* node){
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

        static TreeNode<T, S>* balanceTree(TreeNode<T, S>* root) {
            root->height = AVLTree::height(root);
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
        
        //A function for deleting a node from a BST tree, used later for remove
        static TreeNode<T, S>* deleteNode(TreeNode<T, S>* root, const S& key){
            if (root == nullptr){
                return root;
            }

            if (key < root->key){
                root->left = deleteNode(root->left, key);
            }

            else if (key > root->key){
                root->right = deleteNode(root->right, key);
            }

            else{

                if (root->left == nullptr and root->right == nullptr)
                    return nullptr;
                
                TreeNode<T, S>* temp;
                if (root->left == nullptr) {
                    return root->right;
                }

                if (root->right == nullptr) {
                    return root->left;
                }

                temp = minNode(root->right);
                root->key = temp->key;
                root->data = temp->data;
                //root->data = shared_ptr<T>(temp->data);
                root->right = deleteNode(root->right, temp->key);
            }

            return root;
        }
        

        static TreeNode<T, S>* minNode(TreeNode<T, S>* node) {
            TreeNode<T, S>* current = node;
            while (current->left != nullptr)
                current = current->left;
            return current;
        }

        static TreeNode<T, S>* maxNode(TreeNode<T, S>* node) {
            TreeNode<T, S>* current = node;
            while (current->right != nullptr)
                current = current->right;
            return current;
        }

        static TreeNode<T, S>* RRRotation(TreeNode<T, S>* root) {
            TreeNode<T, S>* oldRoot = root;
            TreeNode<T, S>* newRoot = root->right;
            oldRoot->right = newRoot->left;
            newRoot->left = oldRoot;
            oldRoot->height = height(oldRoot);
            newRoot->height = height(newRoot);
            return newRoot;
        }

        static TreeNode<T, S>* LLRotation(TreeNode<T, S>* root) {
            TreeNode<T, S>* oldRoot = root;
            TreeNode<T, S>* newRoot = root->left;
            oldRoot->left = newRoot->right;
            newRoot->right = oldRoot;
            oldRoot->height = height(oldRoot);
            newRoot->height = height(newRoot);
            return newRoot;
        }

        static TreeNode<T, S>* LRRotation(TreeNode<T, S>* root) {
            TreeNode<T, S>* oldRoot = root;
            TreeNode<T, S>* node = root->left;
            TreeNode<T, S>* newRoot = root->left->right;
            oldRoot->left = newRoot->right;
            node->right = newRoot->left;
            newRoot->right = oldRoot;
            newRoot->left = node;
            oldRoot->height = height(oldRoot);
            node->height = height(node);
            newRoot->height = height(newRoot);
            return newRoot;
        }

        static TreeNode<T, S>* RLRotation(TreeNode<T, S>* root) {
            TreeNode<T, S>* oldRoot = root;
            TreeNode<T, S>* node = root->right;
            TreeNode<T, S>* newRoot = root->right->left;
            oldRoot->right = newRoot->left;
            node->left = newRoot->right;
            newRoot->left = oldRoot;
            newRoot->right = node;
            oldRoot->height = height(oldRoot);
            node->height = height(node);
            newRoot->height = height(newRoot);
            return newRoot;
        }

    public:
        TreeNode<T, S>* root;

        AVLTree();
        ~AVLTree();
        TreeNode<T, S>* insert(TreeNode<T, S>* root, T* data, const S& key);
        TreeNode<T, S>* remove(TreeNode<T, S>* root, const S& key);

        class KeyAlreadyExists : public std::exception {};
};

template<class T, class S>
AVLTree<T, S>::AVLTree(): root(nullptr) {}

template<class T, class S>
AVLTree<T, S>::~AVLTree() {
    destruct(this->root);
}



template<class T, class S>
TreeNode<T, S>* AVLTree<T, S>::insert(TreeNode<T, S>* root, T* data, const S& key) {
    if(root == nullptr) {
        return new TreeNode<T, S>(data, key);
    }
    if(key < root->key) { //locate correct insertion position
        root->left = insert(root->left, data, key);
    }
    else if (key > root->key) {
        root->right = insert(root->right, data, key);
    }
    else { //same keys - illegal
        //throw AVLTree::KeyAlreadyExists();
    }
    return AVLTree::balanceTree(root);
}

/*
template<class T, class S>
TreeNode<T, S>* AVLTree<T, S>::remove(TreeNode<T, S>* root, const S& key) {
    if(root->left == nullptr && root->right == nullptr){
        if (this->root->key == root->key){
            this->root = nullptr;
        }
        if(root->key == key) {
            root = nullptr;
            return nullptr;
        }
        
        return root;
    }
    TreeNode<T, S>* temp;
    if(key > root->key) {
        root->right = remove(root->right, key);
    }
    else if(key < root->key) {
        root->left = remove(root->left, key);
    }
    else {
        if(root->left != nullptr) {
            temp = maxNode(root->left);
            root->key = temp->key; //DONT FORGET TO ADD DATA
            root->left = remove(root->left, temp->key);
        }
        else {
            temp = minNode(root->right);
            root->key = temp->key; //DONT FORGET TO ADD DATA
            root->right = remove(root->right, temp->key);
        }
    }
    int balance = AVLTree::balanceFactor(root);
    if(balance == 2) {
        if(balanceFactor(root->left) >= 0) {
            root = LLRotation(root);
        }
        else {
            root = LRRotation(root);
        }
    }
    if(balance == -2) {
        if(balanceFactor()) {
            
        }
    }
    if(balance == 2 && balanceFactor(root->left) == 1) { //left left rotation
        root = LLRotation(root);
    }
    else if(balance == 2 && balanceFactor(root->left) == -1) { //left right rotation
        root = LRRotation(root);
    }
    else if(balance == 2 && balanceFactor(root->left) == 0) { //left left rotation
        root = LLRotation(root);
    }

    
    else if(balance == -2 && balanceFactor(root->right) == -1) { //right right rotation
        root = RRRotation(root);
    }
    else if(balance == -2 && balanceFactor(root->right) == 1) { //right left rotation
        root = RLRotation(root);
    }
    else if(balance == -2 && balanceFactor(root->right) == 0){ //left left rotation
        root = LLRotation(root);
    }
    return root;
} */


template<class T, class S>
TreeNode<T, S>* AVLTree<T, S>::remove(TreeNode<T, S>* root, const S& key) {
    deleteNode(root, key);

    return balanceTree(root);

    int balance = AVLTree::balanceFactor(root);
    if(balance == 2 && AVLTree::balanceFactor(root->left) == 1) { //left left rotation
        root = AVLTree::LLRotation(root);
    }
    else if(balance == 2 && AVLTree::balanceFactor(root->left) == -1) { //left right rotation
        root = AVLTree::LRRotation(root);
    }
    else if(balance == 2 && AVLTree::balanceFactor(root->left) == 0) { //left left rotation
        root = AVLTree::LLRotation(root);
    }
    else if(balance == -2 && AVLTree::balanceFactor(root->right) == -1) { //right right rotation
        root = AVLTree::RRRotation(root);
    }
    else if(balance == -2 && AVLTree::balanceFactor(root->right) == 1) { //right left rotation
        root = AVLTree::RLRotation(root);
    }
    else if(balance == -2 && AVLTree::balanceFactor(root->right) == 0){ //left left rotation
        root = AVLTree::LLRotation(root);
    }
    return root;

}

#endif

//AVLTree<Player> playerTree;
//playerTree.insert(playerTree.root, )