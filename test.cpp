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


int main() {
    
    AVLTree<int, int> a;
    AVLTree<int, int> b;
    AVLTree<int, int> merged;
    int chooseTree = 1;
    int c,x;
    do{
        //system("clear");
        here:
        cout<<"\n1.Display Tree";
        cout<<"\n2.Display Size of Tree";
        cout<<"\n3.Insert Random Number";
        cout<<"\n4.Delete";
        cout<<"\n5.Switch Tree to Work On (Currently - Tree " << chooseTree << ")";
        cout<<"\n6.Merge the Trees and Display";
        cout<<"\n0.Exit\n";
        cout<<"\nChoice: ";

        cin>>c;

        std::random_device rd;     // Only used once to initialise (seed) engine
        std::mt19937 rng(rd());    // Random-number engine used (Mersenne-Twister in this case)
        std::uniform_int_distribution<int> uni(-20,40); // Guaranteed unbiased

        switch (c)
        {
            case 1:
            {   
                (chooseTree == 1) ? print2D(a.root) : print2D(b.root);
                goto here;
                break;
            }

            case 2:
            {
                int size;
                (chooseTree == 1) ? size = a.getSize() : size = b.getSize();
                cout << "\n" << "Size of the tree is " << size << "\n";
                goto here;
                break;
            }
                    
            case 3:
            {
                //cout<<"\nEnter no. ";
                //cin>>x;
                auto random_integer = uni(rng);
                x = random_integer;
                int * y = new int(x);
                (chooseTree == 1) ? a.insert(y, x) : b.insert(y, x);
                break;
            }
            
            case 4:
            {
                cout<<"\nWhat to delete? ";
                cin>>x;
                (chooseTree == 1) ? a.remove(x) : b.remove(x);
                break;
            }

            case 5:
            {
                (chooseTree == 1) ? chooseTree = 2 : chooseTree = 1;
                break;
            }

            case 6:
            {
                AVLTree<int, int>::merge(a, b, merged);
                print2D(merged.root);
                goto here;
                return 0;
            }
                
            case 0:
            {
                break;
            }
        }

     } while(c!=0);
}