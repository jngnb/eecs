#include "diameter.h"
#include <functional>
#include <algorithm>

int height(const BinaryTreeNode * tree){
    if (!tree){
        return 0;
    }
    else{
        return std::max(height(tree->left), height(tree->right)) + 1;
    }
}

int diameter(const BinaryTreeNode* tree) {
    
    if (!tree){
        return 0;
    }
    else{
        int potential = std::max(diameter(tree->left), diameter(tree->right));
        int current = height(tree->left) + height(tree->right);
        
        return std::max(potential, current);
    }
}
