#include "AVLTree.h"
#include <iostream>

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

struct testing {
    public:
    int a;
    int b;
};


int main() {
    AVLTree<int, int> b;
    int c,x;
    //testing a = {1,2};
    //testing g = {3,4};
    //shared_ptr<int> one(new int(1));
    //shared_ptr<int> two(new int(2));
    //one = two;
    do{
        cout<<"\n1.Display Tree";
        cout<<"\n2.Insert";
        cout<<"\n3.Delete\n";
        cout<<"\n0.Exit\n";
        cout<<"\nChoice: ";

        cin>>c;

        switch (c)
        {
        case 1:
        {
            print2D(b.root);
            // to print the tree in level order
            break;
        }
                  
        case 2:
        {
            cout<<"\nEnter no. ";
            cin>>x;
            int * y = new int(0);
            b.root = b.insert(b.root, y, x);
            break;
        }
        
        case 3:
        {
            cout<<"\nWhat to delete? ";
            cin>>x;
            b.root = b.remove(b.root, x);
            break;
        }
            
        case 0:
        {
            break;
        }
        }

     } while(c!=0);
}