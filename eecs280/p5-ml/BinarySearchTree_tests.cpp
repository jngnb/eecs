// Project UID db1f506d06d84ab787baf250c265e24e

#include "BinarySearchTree.h"
#include "unit_test_framework.h"
#include <iostream>

using namespace std;

TEST(test_public) {
  BinarySearchTree<int> tree;

  tree.insert(5);

  ASSERT_TRUE(tree.size() == 1);
  ASSERT_TRUE(tree.height() == 1);

  ASSERT_TRUE(tree.find(5) != tree.end());

  tree.insert(7);
  tree.insert(3);

  ASSERT_TRUE(tree.check_sorting_invariant());
  ASSERT_TRUE(*tree.max_element() == 7);
  ASSERT_TRUE(*tree.min_element() == 3);
  ASSERT_TRUE(*tree.min_greater_than(5) == 7);

  ostringstream oss_preorder;
  tree.traverse_preorder(oss_preorder);
  ASSERT_TRUE(oss_preorder.str() == "5 3 7 ");

  ostringstream oss_inorder;
  tree.traverse_inorder(oss_inorder);
  ASSERT_TRUE(oss_inorder.str() == "3 5 7 ");
}

TEST(test_empty){
    BinarySearchTree<int> tree;
    ASSERT_TRUE(tree.empty());
    
    tree.insert(0);
    ASSERT_TRUE(!tree.empty());
}

TEST(test_size_height_min_max){
    BinarySearchTree<int> tree;
    ASSERT_TRUE(tree.size() == 0);

    ASSERT_TRUE(tree.min_element() == tree.end());
    ASSERT_TRUE(tree.max_element() == tree.end());

    auto it = tree.insert(1);
    ASSERT_TRUE(tree.size() == 1);
    ASSERT_TRUE(tree.height() == 1);

    ASSERT_TRUE(tree.min_element() == it);
    ASSERT_TRUE(*(tree.min_element()) == 1);
    ASSERT_TRUE(tree.max_element() == it);
    ASSERT_TRUE(*(tree.max_element()) == 1);

    auto it0 = tree.insert(0);
    ASSERT_TRUE(tree.size() == 2);
    ASSERT_TRUE(tree.height() == 2);

    tree.insert(3);
    ASSERT_TRUE(tree.size() == 3);
    ASSERT_TRUE(tree.height() == 2);

    tree.insert(2);
    ASSERT_TRUE(tree.size() == 4);
    ASSERT_TRUE(tree.height() == 3);

    auto it4 = tree.insert(4);
    ASSERT_TRUE(tree.size() == 5);
    ASSERT_TRUE(tree.height() == 3);

    ASSERT_TRUE(tree.min_element() == it0);
    ASSERT_TRUE(*(tree.min_element()) == 0);
    ASSERT_TRUE(tree.max_element() == it4);
    ASSERT_TRUE(*(tree.max_element()) == 4);

}

TEST(test_copy_nodes){
    BinarySearchTree<int> tree;
    tree.insert(2);
    tree.insert(1);
    tree.insert(3);
    BinarySearchTree<int> tree2(tree);

    ASSERT_TRUE(tree.to_string() == tree2.to_string())

    auto it = tree.begin();//1
    auto it2 = tree2.begin();//1

    ++it;//2
    ++it2;//2
    ASSERT_TRUE(*it == 2);
    ASSERT_TRUE(*it == *it2);
    ++it;//3
    ++it2;//3
    ASSERT_TRUE(*it == 3);
    ASSERT_TRUE(*it == *it2);
    ++it;
    ASSERT_TRUE(it == tree.end());

}

TEST(test_find){
    BinarySearchTree<int> tree;
    ASSERT_TRUE(tree.find(0) == tree.end());

    tree.insert(2);
    tree.insert(0);
    tree.insert(3);
    auto it = tree.begin();
    ++(++it); //points to 3
    
    ASSERT_TRUE(*(tree.find(3)) == 3);
    ASSERT_TRUE(tree.find(3) == it);

    tree.insert(1);
    it = tree.begin();
    ++it;//1
    ASSERT_TRUE(*(tree.find(1)) == 1);
    ASSERT_TRUE(tree.find(1) == it);

    ASSERT_TRUE(tree.find(5) == tree.end());

}

TEST(test_insert_preorder_inorder){
    BinarySearchTree<int> tree;

    auto it = tree.insert(1);
    ASSERT_TRUE(it == tree.find(1));

    tree.insert(0);

    it = tree.insert(3);
    ASSERT_TRUE(it == tree.find(3));

    it = tree.insert(2);
    ASSERT_TRUE(it == tree.find(2));
    ostringstream oss_tree;
    oss_tree << tree;
    ASSERT_TRUE(oss_tree.str() == "[ 0 1 2 3 ]");

    it = tree.insert(4);
    ASSERT_TRUE(it == tree.find(4));

    ostringstream oss_preorder;
    tree.traverse_preorder(oss_preorder);
    ASSERT_TRUE(oss_preorder.str() == "1 0 3 2 4 ");

    ostringstream oss_inorder;
    tree.traverse_inorder(oss_inorder);
    ASSERT_TRUE(oss_inorder.str() == "0 1 2 3 4 ");

}

TEST(test_check_sorting_invariant){
    BinarySearchTree<int> tree;
    ASSERT_TRUE(tree.check_sorting_invariant());

    tree.insert(1);
    ASSERT_TRUE(tree.check_sorting_invariant());

    tree.insert(0);
    *tree.begin() = 2;
    ASSERT_FALSE(tree.check_sorting_invariant());

    *tree.begin() = 1;
    ASSERT_FALSE(tree.check_sorting_invariant());

    *tree.begin() = 0;
    ASSERT_TRUE(tree.check_sorting_invariant());

    tree.insert(3);
    ASSERT_TRUE(tree.check_sorting_invariant());
    *(++(++tree.begin())) = -1;
    ASSERT_FALSE(tree.check_sorting_invariant());
    
    *(++(++tree.begin())) = 6;
    tree.insert(4);
    tree.insert(3);
    tree.insert(5);

    ASSERT_TRUE(tree.check_sorting_invariant());
    *(++(++(++(++tree.begin())))) = 2;

    ASSERT_FALSE(tree.check_sorting_invariant());
    
}

TEST(test_min_greater_than){
    BinarySearchTree<int> tree;
    ASSERT_TRUE(tree.min_greater_than(0) == tree.end());

    auto it = tree.insert(1);
    ASSERT_TRUE(tree.min_greater_than(1) == tree.end());
    ASSERT_TRUE(tree.min_greater_than(0) == tree.begin());
    ASSERT_TRUE(tree.min_greater_than(0) == it);

    auto it3 = tree.insert(3);
    ASSERT_TRUE(tree.min_greater_than(1) == (++tree.begin()));
    ASSERT_TRUE(tree.min_greater_than(1) == it3);
    ASSERT_TRUE(tree.min_greater_than(3) == tree.end());
    ASSERT_TRUE(tree.min_greater_than(4) == tree.end());

    tree.insert(0);
    ASSERT_TRUE(tree.min_greater_than(1) == (++(++tree.begin())));
    ASSERT_TRUE(tree.min_greater_than(1) == it3);

    tree.insert(2);
    ASSERT_TRUE(tree.min_greater_than(2) == (++(++(++tree.begin()))));
    ASSERT_TRUE(tree.min_greater_than(2) == it3);


}

TEST_MAIN()
