#ifndef TreeNode_h
#define TreeNode_h 

    class TreeNode {
        public:
            int val;
            TreeNode* left;
            TreeNode* right;
            int height;

            TreeNode();
            TreeNode(int val);
            TreeNode(const TreeNode& other) = default;
            ~TreeNode() = default;
            TreeNode& operator=(const TreeNode& other) = default;
    };

#endif