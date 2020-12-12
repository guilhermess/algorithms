//
// Created by Guilherme Schlinker on 11/1/20.
//

#include <gtest/gtest.h>
#include <trees/bst.h>
#include <trees/bst.ipp>
#include <string>
#include <sstream>

namespace trees::test {

template <typename T>
std::string print_const(const BST<T> &bst) {
  std::ostringstream ss;
  for (auto &itr : bst) {
    ss << itr << " ";
  }
  return ss.str();
}

template <typename T>
std::string print_non_const(BST<T> &bst) {
  std::ostringstream ss;
  for (auto &itr : bst) {
    ss << itr << " ";
  }
  return ss.str();
}

TEST(BST, insert_and_print_non_const) {
  BST<int> bst;
  bst.insert(10);
  bst.insert(5);
  bst.insert(7);
  bst.insert(3);
  bst.insert(13);
  bst.insert(15);
  auto str = print_non_const(bst);
  EXPECT_EQ(str, "3 5 7 10 13 15 ");
}

TEST(BST, insert_and_print_const) {
  BST<int> bst;
  bst.insert(10);
  bst.insert(5);
  bst.insert(7);
  bst.insert(3);
  bst.insert(13);
  bst.insert(15);
  auto str = print_const(bst);
  EXPECT_EQ(str, "3 5 7 10 13 15 ");
}

TEST(BST, empty_bst_size_0) {
  BST<int> bst;
  EXPECT_EQ(bst.size(), 0);
}

TEST(BST, insert_6_and_check_size) {
  BST<int> bst;
  bst.insert(10);
  bst.insert(5);
  bst.insert(7);
  bst.insert(3);
  bst.insert(13);
  bst.insert(15);
  EXPECT_EQ(bst.size(), 6);
}

TEST(BST, insert_1_and_check_size) {
  BST<int> bst;
  bst.insert(-10);
  EXPECT_EQ(bst.size(), 1);
}

TEST(BST, insert_1_string_and_check_size) {
  BST<std::string> bst;
  bst.insert("one");
  EXPECT_EQ(bst.size(), 1);
}

TEST(BST, insert_string_and_print_const) {
  BST<std::string> bst;
  bst.insert("e");
  bst.insert("b");
  bst.insert("a");
  bst.insert("c");
  bst.insert("g");
  bst.insert("f");
  bst.insert("d");
  auto str = print_const(bst);
  EXPECT_EQ(str, "a b c d e f g ");
}

TEST(BST, insert_string_and_print_non_const) {
  BST<std::string> bst;
  bst.insert("e");
  bst.insert("b");
  bst.insert("a");
  bst.insert("c");
  bst.insert("g");
  bst.insert("f");
  bst.insert("d");
  auto str = print_non_const(bst);
  EXPECT_EQ(str, "a b c d e f g ");
}

TEST(BST, insert_6_numbers_and_find_exist) {
  BST<int> bst;
  bst.insert(10);
  bst.insert(5);
  bst.insert(7);
  bst.insert(3);
  bst.insert(13);
  bst.insert(15);
  auto itr = bst.find(7);
  ASSERT_NE(itr, bst.end());
  EXPECT_EQ(*itr, 7);
}

TEST(BST, insert_6_numbers_and_find_non_existent) {
  BST<int> bst;
  bst.insert(10);
  bst.insert(5);
  bst.insert(7);
  bst.insert(3);
  bst.insert(13);
  bst.insert(15);
  auto itr = bst.find(8);
  EXPECT_EQ(itr, bst.end());
}

TEST(BST, convert_itr_to_const) {
  BST<int> bst;
  bst.insert(10);
  bst.insert(5);
  bst.insert(7);
  bst.insert(3);
  bst.insert(13);
  bst.insert(15);
  BST<int>::iterator itr = bst.find(13);
  BST<int>::const_iterator const_itr{itr};
  EXPECT_EQ(*const_itr, 13);
}

TEST(BST, insert_and_erase_child_left_and_right) {
  BST<int> bst;
  bst.insert(1);
  bst.insert(5);
  bst.insert(3);
  bst.insert(7);
  bst.insert(2);
  bst.insert(4);
  bst.insert(6);
  bst.insert(8);
  auto itr = bst.find(5);
  auto next_itr = bst.erase(itr);
  EXPECT_EQ(*next_itr, 6);
  EXPECT_EQ(print_const(bst), "1 2 3 4 6 7 8 ");
}

TEST(BST, insert_and_erase_child_right_only) {
  BST<int> bst;
  bst.insert(1);
  bst.insert(5);
  bst.insert(7);
  bst.insert(6);
  bst.insert(8);
  auto itr = bst.find(5);
  auto next_itr = bst.erase(itr);
  EXPECT_EQ(*next_itr, 6);
  EXPECT_EQ(print_const(bst), "1 6 7 8 ");
}

TEST(BST, insert_and_erase_child_left_only) {
  BST<int> bst;
  bst.insert(1);
  bst.insert(5);
  bst.insert(4);
  bst.insert(2);
  bst.insert(3);
  auto itr = bst.find(5);
  auto next_itr = bst.erase(itr);
  EXPECT_EQ(next_itr, bst.end());
  EXPECT_EQ(print_const(bst), "1 2 3 4 ");
}

TEST(BST, insert_and_erase_root_child_right_only) {
  BST<int> bst;
  bst.insert(1);
  bst.insert(5);
  bst.insert(4);
  bst.insert(6);
  auto itr = bst.find(1);
  auto next_itr = bst.erase(itr);
  EXPECT_EQ(*next_itr, 4);
  EXPECT_EQ(print_const(bst), "4 5 6 ");
}

TEST(BST, insert_and_erase_root_child_left_and_right) {
  BST<int> bst;
  bst.insert(5);
  bst.insert(3);
  bst.insert(7);
  bst.insert(4);
  bst.insert(2);
  bst.insert(6);
  bst.insert(8);
  auto itr = bst.find(5);
  auto next_itr = bst.erase(itr);
  EXPECT_EQ(print_const(bst), "2 3 4 6 7 8 ");
  EXPECT_EQ(*next_itr, 6);
}

TEST(BST, insert_and_erase_by_value) {
  BST<int> bst;
  bst.insert(5);
  bst.insert(3);
  bst.insert(7);
  bst.insert(4);
  bst.insert(2);
  bst.insert(6);
  bst.insert(8);
  EXPECT_EQ(bst.erase(3), 1);
  EXPECT_EQ(bst.size(), 6);
  EXPECT_EQ(print_const(bst), "2 4 5 6 7 8 ");
}


}