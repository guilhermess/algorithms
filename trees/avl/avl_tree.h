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
  explicit AVLNode();
  [[nodiscard]] char balance() const;
  void balance(char value);

 private:
  char balance_;

};
}

template<typename T, typename Comparator = std::less<T>>
class AVLTree : public BST<T, Comparator, detail::AVLNode<T>> {
 public:
  using iterator = typename BST<T,Comparator,detail::AVLNode<T>>::iterator;
  using const_iterator = typename BST<T,Comparator,detail::AVLNode<T>>::const_iterator;
  using value_type = T;

  explicit AVLTree(const Comparator &key_comp = Comparator());

  std::pair<iterator,bool> insert(value_type &&value) override;

 private:
};

}
#endif //ALGORITHMS_TREES_AVL_AVLTREE_H_
