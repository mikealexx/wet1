#include "AVLTree.h"
#include <iostream>
#include <random>

using namespace std;

template<class T, class S>
void print2DUtil(TreeNode<T, S>* root, int space)
{
    // Base case
    if (root == NULL)
        return;
 
    // Increase distance between levels
    space += 10;
 
    // Process right child first
    print2DUtil(root->right, space);
 
    // Print current node after space
    // count
    cout << endl;
    for (int i = 10; i < space; i++)
        cout << " ";
    cout << root->key << "\n";
 
    // Process left child
    print2DUtil(root->left, space);
}
 
// Wrapper over print2DUtil()
void print2D(TreeNode<int, int>* root)
{
    // Pass initial space count as 0
    print2DUtil(root, 0);
}

int main()