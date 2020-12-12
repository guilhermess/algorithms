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
    if (balance_ > 0 && !this->right())
      assert(false);
    else if (balance_ < 0 && !this->left())
      assert(false);
  }

  [[nodiscard]] inline bool check_balance() const {
    auto left_height = (this->left()) ? 1 + this->left()->height() : 0;
    auto right_height = (this->right()) ? 1 + this->right()->height() : 0;
    return (right_height - left_height) == balance_ && std::abs(balance_) < 2;
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

  [[nodiscard]] inline bool check_balance() const {
    return check_balance(this->root());
  }

  [[nodiscard]] inline bool check_consistency() const {
    return check_consistency(this->root());
  };

 private:
  NodeType *rotate_left(NodeType *subroot, NodeType *right);
  NodeType *rotate_right(NodeType *subroot, NodeType *left);

  inline bool check_balance(NodeType *node) const {
    if (node) {
      if (!node->check_balance())
        return node->check_balance() && check_balance(node->left()) && check_balance(node->right());
    }
    return true;
  }
  inline bool check_consistency(NodeType *node) const {
    if (node) {
      bool left_consistent =
          !(node->left()) || (node->left()->parent() == node && check_consistency(node->left()));
      bool right_consistent =
          !(node->right()) || (node->right()->parent() == node && check_consistency(node->right()));
      return left_consistent && right_consistent;
    }
    return true;
  }

  void update_path_balance_insert(const_iterator position);
  void update_path_balance_erase(const_iterator position, int child_offset);
  std::pair<bool, NodeType *> update_path_instance_erase(NodeType *parent, NodeType *node);

};

}
#endif //ALGORITHMS_TREES_AVL_AVLTREE_H_
