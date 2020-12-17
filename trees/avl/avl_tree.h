/*
MIT License

Copyright (c) 2020 Guilherme Simoes Schlinker

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef ALGORITHMS_TREES_AVL_AVLTREE_H_
#define ALGORITHMS_TREES_AVL_AVLTREE_H_

#include <memory>
#include <trees/bst.h>

namespace trees::avl {

namespace detail {

template<typename T>
struct AVLNodeTraits;

template<typename T, typename NodeTraits = avl::detail::AVLNodeTraits<T>>
class AVLNode : public trees::detail::BSTNode<T, NodeTraits> {
  using NodeType = typename NodeTraits::NodeType;
 public:
  inline explicit AVLNode(T &&data,
                          NodeType *parent = nullptr,
                          std::unique_ptr<NodeType> left = nullptr,
                          std::unique_ptr<NodeType> right = nullptr)
      : trees::detail::BSTNode<T, NodeTraits>(std::forward<T>(data),
                                              parent,
                                              std::move(left),
                                              std::move(right)),
        balance_{0} {}

  inline AVLNode(AVLNode const &src,
                 AVLNode *parent) : trees::detail::BSTNode<T, NodeTraits>(src, parent),
                                    balance_{src.balance_} {}

  [[nodiscard]] inline char balance() const { return balance_; }
  inline void balance(char value) {
    balance_ = value;
  }

  [[nodiscard]] inline virtual bool operator==(NodeType const &other) const {
    return trees::detail::BSTNode<T, NodeTraits>::operator==(other)
        && balance() == other.balance();
  }
 private:
  char balance_;
};

template<typename T>
struct AVLNodeTraits {
  using NodeType = typename trees::avl::detail::AVLNode<T, AVLNodeTraits>;
};

}

template<typename T, typename Comparator = std::less<T>>
class AVLTree : public BST<T, Comparator, detail::AVLNode<T>> {
 public:
  using iterator = typename BST<T, Comparator, detail::AVLNode<T>>::iterator;
  using const_iterator = typename BST<T, Comparator, detail::AVLNode<T>>::const_iterator;
  using value_type = typename BST<T, Comparator, detail::AVLNode<T>>::value_type;
  using NodeType = typename detail::AVLNode<T>;
  using BST<T, Comparator, detail::AVLNode<T>>::erase;

  explicit AVLTree(Comparator const &comp = Comparator());
  inline AVLTree(AVLTree &&src) noexcept: BST<T, Comparator, detail::AVLNode<T>>(std::move(src)) {}
  inline AVLTree(AVLTree const &src) : BST<T, Comparator, detail::AVLNode<T>>(src) {}

  std::pair<iterator, bool> insert(value_type value) override;
  iterator erase(const_iterator position) override;

  [[nodiscard]] char balance(const_iterator position);
  [[nodiscard]] char balance(value_type const &value);

 private:
  NodeType *rotate_left(NodeType *subroot, NodeType *right);
  NodeType *rotate_right(NodeType *subroot, NodeType *left);

  void update_path_balance_insert(const_iterator position);
  void update_path_balance_erase(const_iterator position, int child_offset);
  std::pair<bool, NodeType *> update_path_instance_erase(NodeType *parent, NodeType *node);

};

}
#endif //ALGORITHMS_TREES_AVL_AVLTREE_H_
