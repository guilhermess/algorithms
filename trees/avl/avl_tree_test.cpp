//
// Created by Guilherme Schlinker on 11/1/20.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <trees/avl/avl_tree.h>
#include <trees/avl/avl_tree.ipp>
#include <vector>
#include <random>
#include <unordered_set>

namespace trees::avl::test {

AVLTree<int> insert_erase_test(std::vector<int> const &data,
                               std::vector<int> const &operation,
                               int height = -2) {
  AVLTree<int> tree;
  std::set<int> numbers_set;
  for (int i = 0; i < operation.size(); ++i)
    if (operation[i] == 0) {
      tree.insert(data[i]);
      numbers_set.insert(data[i]);
      assert(tree.check_balance());
    } else {
      tree.erase(data[i]);
      numbers_set.erase(data[i]);
      assert(tree.check_balance());
    }
  std::vector<int> tree_output(tree.begin(), tree.end());
  std::vector<int> tree_data(numbers_set.begin(), numbers_set.end());
  EXPECT_EQ(tree.size(), tree_data.size());
  EXPECT_EQ(tree_data, tree_output);
  if (height > -2)
    EXPECT_EQ(tree.height(), height);
  return tree;
}

AVLTree<int> insert_test(std::vector<int> const &data, int height = -2) {
  AVLTree<int> tree;
  for (auto &td : data)
    tree.insert(td);
  std::vector<int> tree_output(tree.begin(), tree.end());
  std::vector<int> tree_data = data;
  std::sort(tree_data.begin(), tree_data.end());
  EXPECT_EQ(tree.size(), tree_data.size());
  EXPECT_EQ(tree_data, tree_output);
  if (height > -2)
    EXPECT_EQ(tree.height(), height);
  return tree;
}

AVLTree<int> erase_test(std::vector<int> const &tree_data,
                        std::vector<int> const &erase_data, int height = -2) {
  AVLTree<int> tree;
  for (auto &td : tree_data)
    tree.insert(td);
  for (auto &td : erase_data)
    tree.erase(td);

  std::set<int> tree_set(tree_data.begin(), tree_data.end());
  for (auto &td : erase_data)
    tree_set.erase(td);

  std::vector<int> tree_output(tree.begin(), tree.end());
  std::vector<int> tree_output_set(tree_set.begin(), tree_set.end());

  EXPECT_EQ(tree.size(), tree_set.size());
  EXPECT_EQ(tree_output_set, tree_output);
  if (height > -2)
    EXPECT_THAT(tree.height(), height);
  return tree;
}

template<typename T>
std::vector<T> generate_random_data(std::size_t size) {
  std::default_random_engine generator(42);
  std::uniform_int_distribution<T> distribution(0, size * size);
  std::unordered_set<T> numbers_set;
  std::vector<T> numbers;
  while (numbers.size() < size) {
    auto number = distribution(generator);
    if (numbers_set.insert(number).second)
      numbers.emplace_back(number);
  }
  return numbers;
}

TEST(AVLTree, insert_1_find_and_check_size) {
  AVLTree<int> tree;
  EXPECT_EQ(tree.size(), 0);
  tree.insert(10);
  EXPECT_EQ(tree.size(), 1);
  auto itr = tree.find(10);
  EXPECT_EQ(*itr, 10);
}

TEST(AVLTree, insert_1_erase_and_check_size) {
  AVLTree<int> tree;
  EXPECT_EQ(tree.size(), 0);
  tree.insert(10);
  tree.erase(10);
  EXPECT_EQ(tree.find(10), tree.end());
  EXPECT_EQ(0, tree.size());
}

TEST(AVLTree, right_higher_left_rotation) {
  auto tree{insert_test({10, 3, 15, 1, 4, 12, 20, 11, 13, 17, 22, 18}, 3)};
  EXPECT_EQ(tree.level(15), 0);
  EXPECT_EQ(tree.balance(15), 0);
  EXPECT_EQ(tree.level(10), 1);
  EXPECT_EQ(tree.balance(10), 0);
  EXPECT_EQ(tree.level(20), 1);
  EXPECT_EQ(tree.balance(20), -1);
  EXPECT_EQ(tree.level(3), 2);
  EXPECT_EQ(tree.balance(3), 0);
  EXPECT_EQ(tree.level(12), 2);
  EXPECT_EQ(tree.balance(12), 0);
  EXPECT_EQ(tree.level(17), 2);
  EXPECT_EQ(tree.balance(17), 1);
  EXPECT_EQ(tree.level(22), 2);
  EXPECT_EQ(tree.balance(22), 0);
  EXPECT_EQ(tree.level(1), 3);
  EXPECT_EQ(tree.balance(1), 0);
  EXPECT_EQ(tree.level(4), 3);
  EXPECT_EQ(tree.balance(4), 0);
  EXPECT_EQ(tree.level(11), 3);
  EXPECT_EQ(tree.balance(11), 0);
  EXPECT_EQ(tree.level(13), 3);
  EXPECT_EQ(tree.balance(13), 0);
  EXPECT_EQ(tree.level(18), 3);
  EXPECT_EQ(tree.balance(18), 0);
}

TEST(AVLTree, left_higher_right_double_rotation) {
  auto tree{insert_test({10, 3, 15, 1, 8, 12, 20, 6, 9, 7}, 3)};
  EXPECT_EQ(tree.level(10), 0);
  EXPECT_EQ(tree.balance(10), -1);
  EXPECT_EQ(tree.level(6), 1);
  EXPECT_EQ(tree.balance(6), 0);
  EXPECT_EQ(tree.level(15), 1);
  EXPECT_EQ(tree.balance(15), 0);
  EXPECT_EQ(tree.level(3), 2);
  EXPECT_EQ(tree.balance(3), -1);
  EXPECT_EQ(tree.level(8), 2);
  EXPECT_EQ(tree.balance(8), 0);
  EXPECT_EQ(tree.level(12), 2);
  EXPECT_EQ(tree.balance(12), 0);
  EXPECT_EQ(tree.level(20), 2);
  EXPECT_EQ(tree.balance(20), 0);
  EXPECT_EQ(tree.level(1), 3);
  EXPECT_EQ(tree.balance(1), 0);
  EXPECT_EQ(tree.level(7), 3);
  EXPECT_EQ(tree.balance(7), 0);
  EXPECT_EQ(tree.level(9), 3);
  EXPECT_EQ(tree.balance(9), 0);
}

TEST(AVLTree, left_right_higher_right_left_double_rotation) {
  auto tree{insert_test({18, 3, 25, 2, 11, 23, 37, 1, 8, 13, 20, 28, 40, 6, 10, 15, 5}, 4)};
  EXPECT_EQ(tree.level(18), 0);
  EXPECT_EQ(tree.balance(18), -1);
  EXPECT_EQ(tree.level(8), 1);
  EXPECT_EQ(tree.balance(8), 0);
  EXPECT_EQ(tree.level(25), 1);
  EXPECT_EQ(tree.balance(25), 0);
  EXPECT_EQ(tree.level(3), 2);
  EXPECT_EQ(tree.balance(3), 0);
  EXPECT_EQ(tree.level(11), 2);
  EXPECT_EQ(tree.balance(11), 1);
  EXPECT_EQ(tree.level(23), 2);
  EXPECT_EQ(tree.balance(23), -1);
  EXPECT_EQ(tree.level(37), 2);
  EXPECT_EQ(tree.balance(37), 0);
  EXPECT_EQ(tree.level(2), 3);
  EXPECT_EQ(tree.balance(2), -1);
  EXPECT_EQ(tree.level(6), 3);
  EXPECT_EQ(tree.balance(6), -1);
  EXPECT_EQ(tree.level(10), 3);
  EXPECT_EQ(tree.balance(10), 0);
  EXPECT_EQ(tree.level(13), 3);
  EXPECT_EQ(tree.balance(13), 1);
  EXPECT_EQ(tree.level(20), 3);
  EXPECT_EQ(tree.balance(20), 0);
  EXPECT_EQ(tree.level(28), 3);
  EXPECT_EQ(tree.balance(28), 0);
  EXPECT_EQ(tree.level(40), 3);
  EXPECT_EQ(tree.balance(40), 0);
}

TEST(AVLTree, root_left_right_higher_right_left_double_rotation) {
  auto tree{insert_test({3, 1, 8, 6, 9, 7}, 2)};
  EXPECT_EQ(tree.level(6), 0);
  EXPECT_EQ(tree.balance(6), 0);
  EXPECT_EQ(tree.level(3), 1);
  EXPECT_EQ(tree.balance(3), -1);
  EXPECT_EQ(tree.level(8), 1);
  EXPECT_EQ(tree.balance(8), 0);
  EXPECT_EQ(tree.level(7), 2);
  EXPECT_EQ(tree.balance(7), 0);
  EXPECT_EQ(tree.level(9), 2);
  EXPECT_EQ(tree.balance(9), 0);
}

TEST(AVLTree, insert_multiple_and_erase_root) {
  erase_test({13, 9, 15, 6, 12, 14, 20, 2, 11, 18, 30}, {13}, 3);
}

TEST(AVLTree, insert_multiple_and_erase_double_rotation) {
  erase_test({9, 6, 15, 2, 7, 13, 20, 1, 12, 14, 18, 30, 11}, {2}, 3);
}

TEST(AVLTree, insert_multiple_and_erase_rotation_0) {
  erase_test({20, 10, 30, 5, 15}, {30}, 2);
}

TEST(AVLTree, insert_multiple_and_erase_rotation_1) {
  erase_test({50, 40, 60, 30, 45, 55, 10}, {55}, 2);
}

TEST(AVLTree, insert_multiple_and_erase_rotation_m1) {
  auto tree{erase_test({50, 40, 60, 45}, {60}, 1)};
  EXPECT_EQ(tree.level(45), 0);
  EXPECT_EQ(tree.balance(45), 0);
  EXPECT_EQ(tree.level(40), 1);
  EXPECT_EQ(tree.balance(40), 0);
  EXPECT_EQ(tree.level(50), 1);
  EXPECT_EQ(tree.balance(50), 0);
}

TEST(AVLTree, insert_increasing) {
  auto tree{std::move(insert_test({1, 2, 3}, 1))};
  EXPECT_EQ(tree.balance(2), 0);
  EXPECT_EQ(tree.level(2), 0);
  EXPECT_EQ(tree.balance(1), 0);
  EXPECT_EQ(tree.level(1), 1);
  EXPECT_EQ(tree.balance(3), 0);
  EXPECT_EQ(tree.level(3), 1);
}

TEST(AVLTree, insert_decreasing) {
  AVLTree<int> tree{std::move(insert_test({3, 2, 1}, 1))};
  EXPECT_EQ(tree.balance(2), 0);
  EXPECT_EQ(tree.level(2), 0);
  EXPECT_EQ(tree.balance(1), 0);
  EXPECT_EQ(tree.level(1), 1);
  EXPECT_EQ(tree.balance(3), 0);
  EXPECT_EQ(tree.level(3), 1);
}

TEST(AVLTree, insert_1_3_2) {
  AVLTree<int> tree{std::move(insert_test({1, 3, 2}, 1))};
  EXPECT_EQ(tree.balance(2), 0);
  EXPECT_EQ(tree.level(2), 0);
  EXPECT_EQ(tree.balance(1), 0);
  EXPECT_EQ(tree.level(1), 1);
  EXPECT_EQ(tree.balance(3), 0);
  EXPECT_EQ(tree.level(3), 1);
}

TEST(AVLTree, insert_3_1_2) {
  insert_test({3, 1, 2}, 1);
}

TEST(AVLTree, insert_3_2_5_4_6_1_7_erase_1) {
  auto tree{erase_test({3, 2, 5, 4, 6, 1, 7}, {1}, 2)};
  EXPECT_EQ(tree.level(5), 0);
  EXPECT_EQ(tree.balance(5), 0);
  EXPECT_EQ(tree.level(3), 1);
  EXPECT_EQ(tree.balance(3), 0);
  EXPECT_EQ(tree.level(6), 1);
  EXPECT_EQ(tree.balance(6), 1);
  EXPECT_EQ(tree.level(2), 2);
  EXPECT_EQ(tree.balance(2), 0);
  EXPECT_EQ(tree.level(4), 2);
  EXPECT_EQ(tree.balance(4), 0);
  EXPECT_EQ(tree.level(7), 2);
  EXPECT_EQ(tree.balance(7), 0);
}

TEST(AVLTree, insert_3_2_5_4_6_7) {
  auto tree{insert_test({3, 2, 5, 4, 6, 7}, 2)};
  EXPECT_EQ(tree.level(5), 0);
  EXPECT_EQ(tree.balance(5), 0);
  EXPECT_EQ(tree.level(3), 1);
  EXPECT_EQ(tree.balance(3), 0);
  EXPECT_EQ(tree.level(6), 1);
  EXPECT_EQ(tree.balance(6), 1);
  EXPECT_EQ(tree.level(2), 2);
  EXPECT_EQ(tree.balance(2), 0);
  EXPECT_EQ(tree.level(4), 2);
  EXPECT_EQ(tree.balance(4), 0);
  EXPECT_EQ(tree.level(7), 2);
  EXPECT_EQ(tree.balance(7), 0);

}

TEST(AVLTree, insert_5_3_6_2_4_7_1_erase_7) {
  auto tree{std::move(erase_test({5, 3, 6, 2, 4, 7, 1}, {7}, 2))};
  EXPECT_EQ(tree.level(3), 0);
  EXPECT_EQ(tree.balance(3), 0);
  EXPECT_EQ(tree.level(2), 1);
  EXPECT_EQ(tree.balance(2), -1);
  EXPECT_EQ(tree.level(5), 1);
  EXPECT_EQ(tree.balance(5), 0);
  EXPECT_EQ(tree.level(1), 2);
  EXPECT_EQ(tree.balance(1), 0);
  EXPECT_EQ(tree.level(4), 2);
  EXPECT_EQ(tree.balance(4), 0);
  EXPECT_EQ(tree.level(6), 2);
  EXPECT_EQ(tree.balance(6), 0);
}

TEST(AVLTree, insert_5_3_6_2_4_1) {
  auto tree{insert_test({5, 3, 6, 2, 4, 1}, 2)};
  EXPECT_EQ(tree.level(3), 0);
  EXPECT_EQ(tree.balance(3), 0);
  EXPECT_EQ(tree.level(2), 1);
  EXPECT_EQ(tree.balance(2), -1);
  EXPECT_EQ(tree.level(5), 1);
  EXPECT_EQ(tree.balance(5), 0);
  EXPECT_EQ(tree.level(1), 2);
  EXPECT_EQ(tree.balance(1), 0);
  EXPECT_EQ(tree.level(4), 2);
  EXPECT_EQ(tree.balance(4), 0);
  EXPECT_EQ(tree.level(6), 2);
  EXPECT_EQ(tree.balance(6), 0);
}

TEST(AVLTree, insert_5_3_10_1_4_8_11_7_9_12_2_6_erase_2) {
  auto tree{erase_test({5, 3, 10, 1, 4, 8, 11, 7, 9, 12, 2, 6}, {2}, 3)};
  EXPECT_EQ(tree.level(8), 0);
  EXPECT_EQ(tree.level(5), 1);
  EXPECT_EQ(tree.level(10), 1);
  EXPECT_EQ(tree.level(3), 2);
  EXPECT_EQ(tree.level(7), 2);
  EXPECT_EQ(tree.level(9), 2);
  EXPECT_EQ(tree.level(11), 2);
  EXPECT_EQ(tree.level(1), 3);
  EXPECT_EQ(tree.level(4), 3);
  EXPECT_EQ(tree.level(6), 3);
  EXPECT_EQ(tree.level(12), 3);

  EXPECT_EQ(tree.balance(8), 0);
  EXPECT_EQ(tree.balance(5), 0);
  EXPECT_EQ(tree.balance(10), 1);
  EXPECT_EQ(tree.balance(3), 0);
  EXPECT_EQ(tree.balance(7), -1);
  EXPECT_EQ(tree.balance(9), 0);
  EXPECT_EQ(tree.balance(11), 1);
  EXPECT_EQ(tree.balance(1), 0);
  EXPECT_EQ(tree.balance(4), 0);
  EXPECT_EQ(tree.balance(6), 0);
  EXPECT_EQ(tree.balance(12), 0);
}

TEST(AVLTree, insert_5_3_10_1_4_8_11_7_9_12_6) {
  auto tree{insert_test({5, 3, 10, 1, 4, 8, 11, 7, 9, 12, 6}, 3)};
  EXPECT_EQ(tree.level(8), 0);
  EXPECT_EQ(tree.level(5), 1);
  EXPECT_EQ(tree.level(10), 1);
  EXPECT_EQ(tree.level(3), 2);
  EXPECT_EQ(tree.level(7), 2);
  EXPECT_EQ(tree.level(9), 2);
  EXPECT_EQ(tree.level(11), 2);
  EXPECT_EQ(tree.level(1), 3);
  EXPECT_EQ(tree.level(4), 3);
  EXPECT_EQ(tree.level(6), 3);
  EXPECT_EQ(tree.level(12), 3);

  EXPECT_EQ(tree.balance(8), 0);
  EXPECT_EQ(tree.balance(5), 0);
  EXPECT_EQ(tree.balance(10), 1);
  EXPECT_EQ(tree.balance(3), 0);
  EXPECT_EQ(tree.balance(7), -1);
  EXPECT_EQ(tree.balance(9), 0);
  EXPECT_EQ(tree.balance(11), 1);
  EXPECT_EQ(tree.balance(1), 0);
  EXPECT_EQ(tree.balance(4), 0);
  EXPECT_EQ(tree.balance(6), 0);
  EXPECT_EQ(tree.balance(12), 0);
}

TEST(AVLTree, insert_8_3_11_2_5_9_12_1_4_6_10_7_erase_10) {
  auto tree{erase_test({8, 3, 11, 2, 5, 9, 12, 1, 4, 6, 10, 7}, {10}, 3)};
  EXPECT_EQ(tree.level(5), 0);
  EXPECT_EQ(tree.level(3), 1);
  EXPECT_EQ(tree.level(8), 1);
  EXPECT_EQ(tree.level(2), 2);
  EXPECT_EQ(tree.level(4), 2);
  EXPECT_EQ(tree.level(6), 2);
  EXPECT_EQ(tree.level(11), 2);
  EXPECT_EQ(tree.level(1), 3);
  EXPECT_EQ(tree.level(7), 3);
  EXPECT_EQ(tree.level(9), 3);
  EXPECT_EQ(tree.level(12), 3);

  EXPECT_EQ(tree.balance(5), 0);
  EXPECT_EQ(tree.balance(3), -1);
  EXPECT_EQ(tree.balance(8), 0);
  EXPECT_EQ(tree.balance(2), -1);
  EXPECT_EQ(tree.balance(4), 0);
  EXPECT_EQ(tree.balance(6), 1);
  EXPECT_EQ(tree.balance(11), 0);
  EXPECT_EQ(tree.balance(1), 0);
  EXPECT_EQ(tree.balance(7), 0);
  EXPECT_EQ(tree.balance(9), 0);
  EXPECT_EQ(tree.balance(12), 0);
}

TEST(AVLTree, insert_8_3_11_2_5_9_12_1_4_6_7) {
  auto tree{insert_test({8, 3, 11, 2, 5, 9, 12, 1, 4, 6, 7}, 3)};
  EXPECT_EQ(tree.level(5), 0);
  EXPECT_EQ(tree.balance(5), 0);
  EXPECT_EQ(tree.level(3), 1);
  EXPECT_EQ(tree.balance(3), -1);
  EXPECT_EQ(tree.level(8), 1);
  EXPECT_EQ(tree.balance(8), 0);
  EXPECT_EQ(tree.level(2), 2);
  EXPECT_EQ(tree.balance(2), -1);
  EXPECT_EQ(tree.level(4), 2);
  EXPECT_EQ(tree.balance(4), 0);
  EXPECT_EQ(tree.level(6), 2);
  EXPECT_EQ(tree.balance(6), 1);
  EXPECT_EQ(tree.level(11), 2);
  EXPECT_EQ(tree.balance(11), 0);
  EXPECT_EQ(tree.level(1), 3);
  EXPECT_EQ(tree.balance(1), 0);
  EXPECT_EQ(tree.level(7), 3);
  EXPECT_EQ(tree.balance(7), 0);
  EXPECT_EQ(tree.level(9), 3);
  EXPECT_EQ(tree.balance(9), 0);
  EXPECT_EQ(tree.level(12), 3);
  EXPECT_EQ(tree.balance(12), 0);
}

TEST(AVLTree, i1e1i8i10i5i6e5i3e1i12i7e8i9i4e4) {
  insert_erase_test({1, 1, 8, 10, 5, 6, 5, 3, 1, 12, 7, 8, 9, 4, 4},
                    {0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0});
}

TEST(AVLTree, i10i6i12i3i7i9i4e4) {
  insert_erase_test({10, 6, 12, 3, 7, 9, 4, 4},
                    {0, 0, 0, 0, 0, 0, 0, 1});
}

TEST(AVLTree, random_avl_vs_bst_height) {
  auto size = 50000;
  auto numbers = generate_random_data<unsigned int>(size);
  AVLTree<unsigned int> avl_tree;
  BST<unsigned int> bs_tree;
  for (auto &number : numbers) {
    avl_tree.insert(number);
    bs_tree.insert(number);
  }
  EXPECT_LE(avl_tree.height(), bs_tree.height());
  std::sort(numbers.begin(), numbers.end());
  ASSERT_EQ(avl_tree.size(), size);
  std::vector<unsigned int> numbers_from_avl_tree(avl_tree.begin(), avl_tree.end());
  std::vector<unsigned int> numbers_from_bs_tree(bs_tree.begin(), bs_tree.end());
  EXPECT_EQ(numbers, numbers_from_avl_tree);
  EXPECT_EQ(numbers, numbers_from_bs_tree);
  EXPECT_LT(avl_tree.height(), 1.44 * std::log2(size));
}

TEST(AVLTree, random_avl_insert_and_erase) {
  auto size = 50000;
  auto numbers = generate_random_data<unsigned int>(size);
  AVLTree<unsigned int> avl_tree;
  std::default_random_engine generator(39);
  std::uniform_int_distribution<int> distribution(1, 10);
  std::set<unsigned int> numbers_set;
  for (int i = 0; i < numbers.size(); ++i) {
    auto number = numbers[i];
    avl_tree.insert(number);
    bool erase = distribution(generator) <= 3 && avl_tree.size() > 0;
    numbers_set.insert(number);
    if (erase) {
      std::uniform_int_distribution<int> index_distribution(0, i);
      auto erase_number = numbers[index_distribution(generator)];
      avl_tree.erase(erase_number);
      numbers_set.erase(erase_number);
    }
  }
  ASSERT_EQ(avl_tree.size(), numbers_set.size());
  std::vector<unsigned int> numbers_from_avl_tree(avl_tree.begin(), avl_tree.end());
  std::vector<unsigned int> numbers_from_set(numbers_set.begin(), numbers_set.end());
  EXPECT_EQ(numbers_from_set, numbers_from_avl_tree);
}

TEST(AVLTree, insert_6_3_8_2_4_7_9_1_erase_4) {
  auto tree{erase_test({6, 3, 8, 2, 4, 7, 9, 1}, {4}, 2)};
  EXPECT_TRUE(tree.check_balance());
  EXPECT_TRUE(tree.check_consistency());
}

TEST(AVLTree, insert_10_5_15_4_6_13_16_3_12_14_erase_16) {
  auto tree{erase_test({10, 5, 15, 4, 6, 13, 16, 3, 12, 14}, {16}, 3)};
  EXPECT_TRUE(tree.check_balance());
  EXPECT_TRUE(tree.check_consistency());
}

TEST(AVLTree, insert_x) {
  auto tree{erase_test({200, 100, 300, 50, 150, 250, 350, 40, 60, 140, 160, 240, 260, 340, 360, 30, 45,
                   55, 130, 230, 245, 255, 330, 35, 235}, {130}, 5)};
  EXPECT_TRUE(tree.check_balance());
  EXPECT_TRUE(tree.check_consistency());
}

TEST(AVLTree, copy_constructor_and_comparison) {
  AVLTree<int> tree;
  AVLTree<int> copy_tree{tree};
  EXPECT_EQ(tree, copy_tree);
}

}