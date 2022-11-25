#include "AVLTree.h"
#include <iostream>

using namespace std;

void print2DUtil(TreeNode* root, int space)
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
    cout << root->val << "\n";
 
    // Process left child
    print2DUtil(root->left, space);
}
 
// Wrapper over print2DUtil()
void print2D(TreeNode* root)
{
    // Pass initial space count as 0
    print2DUtil(root, 0);
}


int main() {
    AVLTree b;
    int c,x;

    do{
        cout<<"\n1.Display levelorder on newline";
        cout<<"\n2.Insert";
        cout<<"\n3.Delete\n";
        cout<<"\n0.Exit\n";
        cout<<"\nChoice: ";

        cin>>c;

        switch (c)
        {
        case 1:
            print2D(b.root);
            // to print the tree in level order
            break;
                  
        case 2:
            cout<<"\nEnter no. ";
            cin>>x;
            b.root = b.insert(b.root,x);
            break;
        
        case 3:
            cout<<"\nWhat to delete? ";
            cin>>x;
            //b.deleteNode(b.root,x);
            break;
            
        case 0:
            break;
        }

     } while(c!=0);
}