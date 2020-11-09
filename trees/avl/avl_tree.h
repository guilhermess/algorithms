//
// Created by Guilherme Schlinker on 10/31/20.
//

#ifndef ALGORITHMS_TREES_AVL_AVLTREE_H_
#define  ALGORITHMS_TREES_AVL_AVLTREE_H_

#include <memory>
#include <trees/bst.h>

namespace trees::avl {

namespace detail {
template<typename T>
class AVLNode : public trees::detail::BSTNode<T> {
 public:
  inline explicit AVLNode(T &&data,
                          AVLNode *parent = nullptr,
                          std::unique_ptr<AVLNode> left = nullptr,
                          std::unique_ptr<AVLNode> right = nullptr)
      : trees::detail::BSTNode<T>(std::forward<T>(data), parent, left, right) {}

  [[nodiscard]] char balance() const;
  void balance(char value);

 private:
  char balance_;

};
}

template<typename T, typename Comparator = std::less<T>>
class AVLTree : public BST<T, Comparator, detail::AVLNode<T>> {
 public:
  using iterator = typename BST<T, Comparator, detail::AVLNode<T>>::iterator;
  using const_iterator = typename BST<T, Comparator, detail::AVLNode<T>>::const_iterator;
  using value_type = typename BST<T, Comparator, detail::AVLNode<T>>::value_type;

  explicit AVLTree(Comparator const &comp = Comparator());

  std::pair<iterator, bool> insert(value_type &&value) override;
  iterator erase(const_iterator position) override;

 private:
};

}
#endif //ALGORITHMS_TREES_AVL_AVLTREE_H_
