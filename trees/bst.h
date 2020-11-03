//
// Created by Guilherme Schlinker on 11/1/20.
//

#ifndef ALGORITHMS_TREES_BST_HXX_
#define ALGORITHMS_TREES_BST_HXX_

#include <memory>

namespace trees {

namespace detail {
template<typename T>
class BSTNode {
 public:
  BSTNode(T &&data,
          std::unique_ptr<BSTNode> left = nullptr,
          std::unique_ptr<BSTNode> right = nullptr) : data_{data}, left_{left}, right_{right} {}

  T const &data() const { return data_; }
  T &data() { return data_; }

  std::unique_ptr<BSTNode> &left() { return left_; }
  std::unique_ptr<BSTNode> &right() { return left_; }

 private:
  T data_;
  std::unique_ptr<BSTNode> left_;
  std::unique_ptr<BSTNode> right_;
};

}

template<typename T, typename Comparator, typename NodeType = detail::BSTNode<T>>
class BST {
 public:
  virtual ~BST() {}

  class iterator {
   public:
    using value_type = T;

  };

 protected:

 private:
  std::unique_ptr<NodeType> root_;
};

}
#endif //ALGORITHMS_TREES_BST_HXX_
