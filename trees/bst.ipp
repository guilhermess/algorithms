
#ifndef ALGORITHMS_TREES_BST_IPP_
#define  ALGORITHMS_TREES_BST_IPP_

#include <trees/bst.h>

namespace trees {

template<typename T, typename Comparator, typename NodeType>
template<bool is_const>
BST<T, Comparator, NodeType>::base_iterator<is_const>::base_iterator(tree_type *bst)
    : base_iterator(bst
                    ? down_leftmost(bst->root())
                    : nullptr) {
}

template<typename T, typename Comparator, typename NodeType>
template<bool is_const>
BST<T, Comparator, NodeType>::base_iterator<is_const>::base_iterator(NodeType *node) : dummy_{} {
  current_ = node;
}

template<typename T, typename Comparator, typename NodeType>
template<bool is_const>
BST<T, Comparator, NodeType>::base_iterator<is_const>::base_iterator() : base_iterator{
    static_cast<NodeType *>(nullptr)} {
}

template<typename T, typename Comparator, typename NodeType>
template<bool is_const>
BST<T, Comparator, NodeType>::base_iterator<is_const>::base_iterator(base_iterator const &src)
    : base_iterator{
    src.current_} {
}

template<typename T, typename Comparator, typename NodeType>
template<bool is_const>
typename BST<T, Comparator, NodeType>::template base_iterator<is_const> &BST<T,
                                                                             Comparator,
                                                                             NodeType>::base_iterator<
    is_const>::operator++() {
  if (current_) {
    auto res = down_leftmost(current_->right());
    if (!res)
      res = up_first_right(current_);
    current_ = res;
  }
  return *this;
}

template<typename T, typename Comparator, typename NodeType>
template<bool is_const>
typename BST<T, Comparator, NodeType>::template base_iterator<is_const> BST<T,
                                                                            Comparator,
                                                                            NodeType>::base_iterator<
    is_const>::operator++(
    int) {
  base_iterator<is_const> retval = *this;
  ++(*this);
  return retval;
}

template<typename T, typename Comparator, typename NodeType>
template<bool is_const>
typename BST<T, Comparator, NodeType>::template base_iterator<is_const> &BST<T,
                                                                             Comparator,
                                                                             NodeType>::base_iterator<
    is_const>::operator--() {
  if (current_) {
    auto res = down_rightmost(current_->left());
    if (!res)
      res = up_first_left(current_);
    current_ = res;
  }
  return *this;
}

template<typename T, typename Comparator, typename NodeType>
template<bool is_const>
typename BST<T, Comparator, NodeType>::template base_iterator<is_const> BST<T,
                                                                            Comparator,
                                                                            NodeType>::base_iterator<
    is_const>::operator--(
    int) {
  base_iterator<is_const> retval = *this;
  --(*this);
  return retval;
}

template<typename T, typename Comparator, typename NodeType>
template<bool is_const>
NodeType *BST<T,
              Comparator,
              NodeType>::base_iterator<is_const>::down_rightmost(NodeType *node) const {
  NodeType *res = nullptr;
  if (node)
    res = node->rightmost();
  return res;
}

template<typename T, typename Comparator, typename NodeType>
template<bool is_const>
NodeType *BST<T,
              Comparator,
              NodeType>::base_iterator<is_const>::down_leftmost(NodeType *node) const {
  NodeType *res = nullptr;
  if (node)
    res = node->leftmost();
  return res;
}

template<typename T, typename Comparator, typename NodeType>
template<bool is_const>
NodeType *BST<T,
              Comparator,
              NodeType>::base_iterator<is_const>::up_first_left(NodeType *node) const {
  auto res = node;
  while (res) {
    if (child_is_right(res->parent(), res))
      return res->parent();
    res = res->parent();
  }
  return nullptr;
}

template<typename T, typename Comparator, typename NodeType>
template<bool is_const>
NodeType *BST<T,
              Comparator,
              NodeType>::base_iterator<is_const>::up_first_right(NodeType *node) const {
  auto res = node;
  while (res) {
    if (child_is_left(res->parent(), res))
      return res->parent();
    res = res->parent();
  }
  return nullptr;
}

template<typename T, typename Comparator, typename NodeType>
template<bool is_const>
bool BST<T,
         Comparator,
         NodeType>::base_iterator<is_const>::child_is_left(NodeType const *parent,
                                                           NodeType const *child) const {
  if (parent && parent->child_is_left(child))
    return true;
  return false;
}

template<typename T, typename Comparator, typename NodeType>
template<bool is_const>
bool BST<T,
         Comparator,
         NodeType>::base_iterator<is_const>::child_is_right(NodeType const *parent,
                                                            NodeType const *child) const {
  if (parent && parent->child_is_right(child))
    return true;
  return false;
}

template<typename T, typename Comparator, typename NodeType>
template<bool is_const>
bool BST<T,
         Comparator,
         NodeType>::base_iterator<is_const>::operator==(const base_iterator<is_const> &other) const {
  return current_ == other.current_;
}

template<typename T, typename Comparator, typename NodeType>
template<bool is_const>
bool BST<T,
         Comparator,
         NodeType>::base_iterator<is_const>::operator!=(const base_iterator<is_const> &other) const {
  return !(*this == other);
}

template<typename T, typename Comparator, typename NodeType>
template<bool is_const>
typename BST<T, Comparator, NodeType>::template base_iterator<is_const>::reference BST<T,
                                                                                       Comparator,
                                                                                       NodeType>::base_iterator<
    is_const>::operator*() {
  if (current_) {
    return current_->data();
  }
  //TODO: review return dummy or throw?
  return dummy_;
}

template<typename T, typename Comparator, typename NodeType>
std::pair<typename BST<T, Comparator, NodeType>::iterator, bool> BST<T,
                                                                     Comparator,
                                                                     NodeType>::insert(value_type value) {
  return insert(std::move(value), root());
}

template<typename T, typename Comparator, typename NodeType>
std::pair<typename BST<T, Comparator, NodeType>::iterator, bool> BST<T,
                                                                     Comparator,
                                                                     NodeType>::insert(value_type &&value,
                                                                                       NodeType *node) {
  if (!node) {
    root_ = std::make_unique<NodeType>(std::forward<value_type>(value));
    ++size_;
    return std::make_pair(iterator(root()), true);
  } else {
    if (!comp_(node->data(), value) && !comp_(value, node->data()))
      return std::make_pair(iterator{}, false);
    else if (comp_(value, node->data())) {
      if (node->left()) return insert(std::forward<value_type>(value), node->left());
      else {
        auto new_node = node->add_left(std::forward<value_type>(value));
        ++size_;
        return std::make_pair(iterator(new_node), true);
      }
    } else {
      if (node->right()) return insert(std::forward<value_type>(value), node->right());
      else {
        auto new_node = node->add_right(std::forward<value_type>(value));
        ++size_;
        return std::make_pair(iterator(new_node), true);
      }
    }
  }
}

template<typename T, typename Comparator, typename NodeType>
typename BST<T, Comparator, NodeType>::const_iterator BST<T,
                                                          Comparator,
                                                          NodeType>::find(value_type const &value) const {
  return const_iterator{find_node(value)};

}

template<typename T, typename Comparator, typename NodeType>
typename BST<T, Comparator, NodeType>::iterator BST<T,
                                                    Comparator,
                                                    NodeType>::find(value_type const &value) {
  return iterator{find_node(value)};

}

template<typename T, typename Comparator, typename NodeType>
NodeType *BST<T, Comparator, NodeType>::find_node(value_type const &value) const {
  return find_node(value, root());
}

template<typename T, typename Comparator, typename NodeType>
NodeType *BST<T, Comparator, NodeType>::find_node(value_type const &value, NodeType *node) const {
  if (!node)
    return nullptr;
  if (!comp_(node->data(), value) && !comp_(value, node->data()))
    return node;
  if (comp_(value, node->data()))
    return find_node(value, node->left());
  else
    return find_node(value, node->right());
}

template<typename T, typename Comparator, typename NodeType>
typename BST<T, Comparator, NodeType>::iterator BST<T,
                                                    Comparator,
                                                    NodeType>::erase(const_iterator position) {
  auto node = position.current();
  iterator itr = end();
  if (!node)
    return itr;
  if (node->left()) {
    if (node->right()) {
      itr = iterator(node->right()->leftmost());
      if (node->left()->right()) {
        auto right_leftmost = node->right()->leftmost();
        right_leftmost->left(std::move(node->left()->right_move()));
      }
      node->left()->right(std::move(node->right_move()));
    }
    if (node->parent()) {
      if (node->parent()->child_is_left(node))
        node->parent()->left(std::move(node->left_move()));
      else
        node->parent()->right(std::move(node->left_move()));
    } else {
      root_ = std::move(node->left_move());
    }
  } else {
    if (node->right()) {
      itr = iterator(node->right()->leftmost());
      if (node->parent()) {
        if (node->parent()->child_is_left(node))
          node->parent()->left(std::move(node->right_move()));
        else
          node->parent()->right(std::move(node->right_move()));
      } else {
        root_ = std::move(node->right_move());
      }
    } else {
      root_ = nullptr;
    }
  }
  --size_;
  return itr;
}

template<typename T, typename Comparator, typename NodeType>
std::size_t BST<T, Comparator, NodeType>::erase(value_type const &value) {
  auto itr = find(value);
  if (itr != end()) {
    erase(itr);
    return 1;
  }
  return 0;
}

template<typename T, typename Comparator, typename NodeType>
typename BST<T, Comparator, NodeType>::iterator BST<T,
                                                    Comparator,
                                                    NodeType>::erase(const_iterator first,
                                                                     const_iterator last) {
  iterator res = end();
  while (first != last) {
    res = erase(first++);
  }
  return res;
}

template<typename T, typename Comparator, typename NodeType>
std::unique_ptr<NodeType> BST<T,
                              Comparator,
                              NodeType>::move_node_and_replace(NodeType *node,
                                                               std::unique_ptr<NodeType> replacement) {
  if (node->parent()) {
    if (node->parent()->child_is_left(node)) {
      std::unique_ptr<NodeType> ret = node->parent()->left_move();
      node->parent()->left(std::move(replacement));
      return ret;
    } else {
      std::unique_ptr<NodeType> ret = node->parent()->right_move();
      node->parent()->right(std::move(replacement));
      return ret;
    }
  } else {
    std::unique_ptr<NodeType> ret = std::move(root_);
    root_ = std::move(replacement);
    root_->parent(nullptr);
    return ret;
  }
}

template<typename T, typename Comparator, typename NodeType>
std::size_t BST<T, Comparator, NodeType>::height() const {
  return height(root_.get());
}

template<typename T, typename Comparator, typename NodeType>
std::size_t BST<T, Comparator, NodeType>::height(NodeType *node) const {
  std::size_t res = 0;
  if (node) {
    return node->height();
  }
  return res;
}

template<typename T, typename Comparator, typename NodeType>
std::size_t BST<T, Comparator, NodeType>::level(const_iterator position) const {
  int level = 0;
  auto node = this->current(position);
  while (node->parent()) {
    ++level;
    node = node->parent();
  }
  return level;
}

template<typename T, typename Comparator, typename NodeType>
std::size_t BST<T, Comparator, NodeType>::level(value_type const &value) const {
  return level(this->find(value));
}

}
#endif
