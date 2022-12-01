#ifndef AVLTree_h
#define AVLTree_h

#include "TreeNode.h"

template<class T, class S>
class AVLTree {
    private:

        //calculate max
        static int max(int a, int b) {
            return (a>b) ? a : b;
        }

        //calculate height of a node
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

            return AVLTree::max(node->left->height, node->right->height) + 1;
        }

        //calculate balance factor of a node
        static int balanceFactor(const TreeNode<T, S>* node) {
            if(node == nullptr) {
                return 0;
            }
            return AVLTree::height(node->left) - AVLTree::height(node->right);
        }

        //helper function for the D'CTOR
        static void destruct(TreeNode<T, S>* node){
            if(node == nullptr){
                return;
            }
            if(node->left == nullptr && node->right == nullptr){
                delete(node);
                return;
            }
            destruct(node->left);
            destruct(node->right);
            delete(node);
        }

        static AVLTree<T, S> merge(AVLTree<T, S>& tree1, AVLTree<T, S>& tree2){
            
        }

        //rebalance the tree using rotations
        static TreeNode<T, S>* balanceTree(TreeNode<T, S>* root) {
            if(root == nullptr) {
                return nullptr;
            }
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

                if (root->left == nullptr && root->right == nullptr) {
                    delete(root);
                    return nullptr;
                }
                
                TreeNode<T, S>* temp;
                if (root->left == nullptr) {
                    temp = root;
                    root = root->right;
                    delete(temp);
                    return root;
                }

                if (root->right == nullptr) {
                    temp = root;
                    root = root->left;
                    delete(temp);
                    return root;
                }

                temp = minNode(root->right);
                root->key = temp->key;
                root->data = temp->data;
                //root->data = shared_ptr<T>(temp->data);
                delete(temp);
                root->right = deleteNode(root->right, temp->key);
            }

            return root;
        }
        
        //return predecessor
        static TreeNode<T, S>* minNode(TreeNode<T, S>* node) {
            TreeNode<T, S>* current = node;
            while (current->left != nullptr)
                current = current->left;
            return current;
        }

        //return successor
        static TreeNode<T, S>* maxNode(TreeNode<T, S>* node) {
            TreeNode<T, S>* current = node;
            while (current->right != nullptr)
                current = current->right;
            return current;
        }

        //right right rotation
        static TreeNode<T, S>* RRRotation(TreeNode<T, S>* root) {
            TreeNode<T, S>* oldRoot = root;
            TreeNode<T, S>* newRoot = root->right;
            oldRoot->right = newRoot->left;
            newRoot->left = oldRoot;
            oldRoot->height = height(oldRoot);
            newRoot->height = height(newRoot);
            return newRoot;
        }

        //left left rotation
        static TreeNode<T, S>* LLRotation(TreeNode<T, S>* root) {
            TreeNode<T, S>* oldRoot = root;
            TreeNode<T, S>* newRoot = root->left;
            oldRoot->left = newRoot->right;
            newRoot->right = oldRoot;
            oldRoot->height = height(oldRoot);
            newRoot->height = height(newRoot);
            return newRoot;
        }

        //left right rotation
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

        //right left rotation
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

        TreeNode<T, S>* insertHelper(TreeNode<T, S>* root, T* data, const S& key) {
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

        TreeNode<T, S>* removeHelper(TreeNode<T, S>* root, const S& key) {
            return AVLTree::balanceTree(deleteNode(root, key));
        }

    public:
        TreeNode<T, S>* root;

        AVLTree() = default;
        ~AVLTree();
        void insert(T* data, const S& key);
        void remove(const S& key);
};

template<class T, class S>
AVLTree<T, S>::AVLTree(): root(nullptr) {}

template<class T, class S>
AVLTree<T, S>::~AVLTree() {
    AVLTree::destruct(this->root);
}



template<class T, class S>
void AVLTree<T, S>::insert(T* data, const S& key) {
    this->root = insertHelper(this->root, data, key);
}

template<class T, class S>
void AVLTree<T, S>::remove(const S& key) {
    this->root = removeHelper(this->root, key);
}

#endif

//AVLTree<Player> playerTree;
//playerTree.insert(playerTree.root, )