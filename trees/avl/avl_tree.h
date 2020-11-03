//
// Created by Guilherme Schlinker on 10/31/20.
//

#ifndef ALGORITHMS_TREES_AVL_AVLTREE_H_
#define ALGORITHMS_TREES_AVL_AVLTREE_H_

#include <memory>
#include <trees/bst.h>

namespace trees::avl {

namespace detail {
template<typename T>
class AVLNode : public trees::detail::BSTNode<T> {
 public:
  explicit AVLNode() {}
  [[nodiscard]] char balance() const { return balance_; }

 private:
  char balance_;

};
}

template<typename T, typename Comparator = std::less<T>>
class AVLTree : public BST<T, Comparator, detail::AVLNode<T>> {
 public:
  explicit AVLTree() {}
  bool insert()

 private:
  std::unique_ptr<detail::AVLNode<T>> root_;
};

}
#endif //ALGORITHMS_TREES_AVL_AVLTREE_H_
