#ifndef TreeNodeID_h
#define TreeNodeID_h

#include <memory>
#include "Player.h"
using namespace std;

template <class T>
class TreeNode {
public:
    shared_ptr<T> data;
    T key;
    TreeNode* left;
    TreeNode* right;
    int height;

    TreeNode() = delete;
    TreeNode(T& key, shared_ptr<T> data);
    TreeNode(const TreeNode& other) = default;
    ~TreeNode() = default;
    TreeNode& operator=(const TreeNode& other) = default;

};

template <class T>
TreeNode<T>::TreeNode(T& key, shared_ptr<T> data):
    data(data),
    key(key),
    left(nullptr),
    right(nullptr),
    height(1)
{}



#endif