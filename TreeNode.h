#ifndef TreeNodeID_h
#define TreeNodeID_h

#include <memory>
//#include "Player.h"
using namespace std;

template <class T, class S>
class TreeNode {
public:
    shared_ptr<T> data;
    T key;
    TreeNode* left;
    TreeNode* right;
    int height;

    TreeNode();
    TreeNode(T* data, const S& key);
    TreeNode(const TreeNode& other) = default;
    ~TreeNode() = default;
    TreeNode& operator=(const TreeNode& other);

};

template <class T, class S>
TreeNode<T, S>::TreeNode():
    data(nullptr),
    key(),
    left(nullptr),
    right(nullptr),
    height(1)
{}

template <class T, class S>
TreeNode<T, S>::TreeNode(T* data, const S& key):
    data(data),
    key(key),
    left(nullptr),
    right(nullptr),
    height(1)
{}

template <class T, class S>
TreeNode<T, S>& TreeNode<T, S>::operator=(const TreeNode<T, S>& other) {
    this->data = other->data;
    this->key = other->key;
    this->left = other->left;
    this->right = other->right;
    this->height = other->height;
}

#endif