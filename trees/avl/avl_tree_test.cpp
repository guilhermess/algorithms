//
// Created by Guilherme Schlinker on 11/1/20.
//

#include <gtest/gtest.h>
#include <trees/avl/avl_tree.h>
#include <trees/avl/avl_tree.ipp>

namespace trees::avl::test {

TEST(AVLTree, construct_avl_tree) {
  AVLTree<int> tree;
  EXPECT_EQ(tree.size(), 0);
  tree.insert(10);
  auto itr = tree.find(10);
  EXPECT_EQ(*itr, 10);
}

}
