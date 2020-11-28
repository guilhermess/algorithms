//
// Created by Guilherme Schlinker on 11/01/20.
//

#ifndef ALGORITHMS_TREES_AVL_AVLTREE_IPP_
#define  ALGORITHMS_TREES_AVL_AVLTREE_IPP_

#include <trees/avl/avl_tree.h>
#include <trees/bst.ipp>

namespace trees::avl {

template<typename T, typename Comparator>
AVLTree<T, Comparator>::AVLTree(Comparator const &comp)
    : BST<T, Comparator, detail::AVLNode<T>>
          (comp) {
}

template<typename T, typename Comparator>
std::pair<typename AVLTree<T, Comparator>::iterator, bool>
AVLTree<T, Comparator>::insert(value_type value) {
  auto[itr, success] = BST<T,
                           Comparator,
                           detail::AVLNode<T>>::insert(std::forward<value_type>(value));
  update_path_balance_insert(itr);
  return std::make_pair(itr, success);
}

template<typename T, typename Comparator>
typename AVLTree<T, Comparator>::iterator
AVLTree<T, Comparator>::erase(const_iterator position) {
  if (position == this->end())
    return this->end();
  auto node = this->current(position);
  auto parent = node->parent();
  if (node->left() && node->right()) {
    auto next = node->right()->leftmost();
    node->data(std::move(next->data_move()));
    erase(const_iterator(next));
    return iterator(node);
  } else if (node->left()) {
    auto prev = node->left();
    node->data(std::move(prev->data_move()));
    erase(const_iterator(prev));
    return ++iterator(node);
  } else if (node->right()) {
    auto next = node->right();
    node->data(std::move(next->data_move()));
    erase(const_iterator(next));
    return iterator(node);
  } else {
    bool update_path = true;
    if (parent && parent->child_is_left(node)) {
      if (parent->balance() == 0)
        update_path = false;
      parent->balance(parent->balance() + 1);
      parent->left(nullptr);
    } else if (parent && parent->child_is_right(node)) {
      if (parent->balance() == 0)
        update_path = false;
      parent->balance(parent->balance() - 1);
      parent->right(nullptr);
    } else {
      this->root(nullptr);
    }
    this->size(this->size() - 1);
    if (update_path)
      update_path_balance_erase(const_iterator(parent));

  }
  return this->end();
}

template<typename T, typename Comparator>
std::pair<bool,
          typename AVLTree<T, Comparator>::NodeType *>
AVLTree<T, Comparator>::update_path_instance_erase(NodeType *parent,
                                                   NodeType *node) {
  if (parent->balance() == 2 && node->balance() >= 0) {
    auto node_balance = node->balance();
    rotate_left(parent, node);
    node->balance(-1 + node_balance);
    parent->balance(1 - node_balance);
    return std::make_pair(true, node);
  } else if (parent->balance() == -2 && node->balance() <= 0) {
    auto node_balance = node->balance();
    rotate_right(parent, node);
    node->balance(1 + node_balance);
    parent->balance(-1 - node_balance);
    assert(parent->check_balance());
    assert(node->check_balance());
    return std::make_pair(true, node);
  } else if (parent->balance() == 2 && node->balance() == -1) {
    auto node_left_balance = node->left()->balance();
    auto node_left = node->left();
    rotate_right(node, node_left);
    node_left->balance(0);
    auto parent_right = parent->right();
    rotate_left(parent, parent_right);
    parent_right->balance(0);
    if (node_left_balance > 0) {
      parent->balance(-1);
      node->balance(0);
    } else {
      if (node_left_balance == 0) {
        parent->balance(0);
        node->balance(0);
      } else {
        parent->balance(0);
        node->balance(1);
      }
    }
    assert(parent->check_balance());
    assert(node->check_balance());
    return std::make_pair(false, nullptr);
  } else if (parent->balance() == -2 && node->balance() == 1) {
    auto node_right_balance = node->right()->balance();
    auto node_right = node->right();
    rotate_left(node, node_right);
    node_right->balance(0);
    auto parent_left = parent->left();
    rotate_right(parent, parent_left);
    parent_left->balance(0);
    if (node_right_balance < 0) {
      parent->balance(1);
      node->balance(0);
    } else {
      if (node_right_balance == 0) {
        parent->balance(0);
        node->balance(0);
      } else {
        parent->balance(0);
        node->balance(-1);
      }
    }
    assert(parent->check_balance());
    assert(node->check_balance());
    return std::make_pair(false, nullptr);
  }
  return std::make_pair(true, nullptr);
}

template<typename T, typename Comparator>
void AVLTree<T, Comparator>::update_path_balance_insert(const_iterator position) {
  if (position == this->end())
    return;
  auto curr = this->current(position);
  while (curr && curr->parent()) {
    auto parent = curr->parent();
    parent->balance(parent->child_is_left(curr) ? parent->balance() - 1 : parent->balance() + 1);
    if (parent->balance() == 0)
      break;
    if (parent->balance() == 2 && curr->balance() == 1) {
      rotate_left(parent, curr);
      parent->balance(0);
      curr->balance(0);
      assert(parent->check_balance());
      assert(curr->check_balance());
      break;
    } else if (parent->balance() == -2 && curr->balance() == -1) {
      rotate_right(parent, curr);
      parent->balance(0);
      curr->balance(0);
      assert(parent->check_balance());
      assert(curr->check_balance());
      break;
    } else if (parent->balance() == 2 && curr->balance() == -1) {
      auto balance_curr_left = curr->left()->balance(); //y
      auto curr_left = curr->left();
      rotate_right(curr, curr_left);
      curr_left->balance(0);
      auto parent_right = parent->right();
      rotate_left(parent, parent_right);
      parent_right->balance(0);
      if (balance_curr_left > 0) {
        parent->balance(-1);
        curr->balance(0);
      } else {
        if (balance_curr_left == 0) {
          parent->balance(0);
          curr->balance(0);
        } else {
          parent->balance(0);
          curr->balance(1);
        }
      }
      assert(parent->check_balance());
      assert(curr->check_balance());
      break;
    } else if (parent->balance() == -2 && curr->balance() == 1) {
      auto balance_curr_right = curr->right()->balance(); //y
      auto curr_right = curr->right();
      rotate_left(curr, curr_right);
      curr_right->balance(0);
      auto parent_left = parent->left();
      rotate_right(parent, parent_left);
      parent_left->balance(0);
      if (balance_curr_right < 0) {
        parent->balance(1);
        curr->balance(0);
      } else {
        if (balance_curr_right == 0) {
          parent->balance(0);
          curr->balance(0);
        } else {
          parent->balance(0);
          curr->balance(-1);
        }
      }
      assert(parent->check_balance());
      assert(curr->check_balance());
      break;
    }
    curr = parent;
  }
}

template<typename T, typename Comparator>
void AVLTree<T, Comparator>::update_path_balance_erase(const_iterator position) {
  if (position == this->end())
    return;
  auto curr = this->current(position);
  while (curr && curr->parent()) {
    auto parent = curr->parent();
    auto other = (parent->child_is_left(curr)) ? parent->right() : parent->left();
    bool parent_balance_is_zero = (parent->balance() == 0);
    parent->balance(parent->child_is_left(curr) ? parent->balance() + 1 : parent->balance() - 1);
    if (parent->balance() == 2 && other->balance() >= 0) {
      auto other_balance = other->balance();
      rotate_left(parent, other);
      other->balance(-1 + other_balance);
      parent->balance(1 - other_balance);
      curr = other;
    } else if (parent->balance() == -2 && other->balance() <= 0) {
      auto other_balance = other->balance();
      rotate_right(parent, other);
      other->balance(1 + other_balance);
      parent->balance(-1 - other_balance);
      curr = other;
      assert(parent->check_balance());
      assert(other->check_balance());
    } else if (parent->balance() == 2 && other->balance() == -1) {
      auto other_left_balance = other->left()->balance();
      auto other_left = other->left();
      rotate_right(other, other_left);
      other_left->balance(0);
      auto parent_right = parent->right();
      rotate_left(parent, parent_right);
      parent_right->balance(0);
      if (other_left_balance > 0) {
        parent->balance(-1);
        other->balance(0);
      } else {
        if (other_left_balance == 0) {
          parent->balance(0);
          other->balance(0);
        } else {
          parent->balance(0);
          other->balance(1);
        }
      }
      assert(parent->check_balance());
      assert(other->check_balance());
      break;
    } else if (parent->balance() == -2 && other->balance() == 1) {
      auto other_right_balance = other->right()->balance();
      auto other_right = other->right();
      rotate_left(other, other_right);
      other_right->balance(0);
      auto parent_left = parent->left();
      rotate_right(parent, parent_left);
      parent_left->balance(0);
      if (other_right_balance < 0) {
        parent->balance(1);
        other->balance(0);
      } else {
        if (other_right_balance == 0) {
          parent->balance(0);
          other->balance(0);
        } else {
          parent->balance(0);
          other->balance(-1);
        }
      }
      assert(parent->check_balance());
      assert(other->check_balance());
      break;
    }
    if (parent_balance_is_zero)
      break;
    curr = curr->parent();
  }
  if (curr && !curr->parent() && curr->balance() == -2) {
    auto other = curr->left();
    if (other->balance() == 1) {
      auto other_right = other->right();
      rotate_left(other, other_right);
      other_right->balance(0);
      auto curr_left = curr->left();
      rotate_right(curr, curr_left);
      curr_left->balance(0);
      curr->balance(0);
      other->balance(0);
      assert(curr->check_balance());
      assert(curr_left->check_balance());
      assert(other->check_balance());
    } else {
      auto other_balance = other->balance();
      rotate_right(curr, other);
      other->balance(1 + other_balance);
      curr->balance(-1 - other_balance);
      assert(curr->check_balance());
      assert(other->check_balance());
    }
  } else if (curr && !curr->parent() && curr->balance() == 2) {
    auto other = curr->right();
    if (other->balance() == -1) {
      auto other_left = other->left();
      rotate_right(other, other_left);
      other_left->balance(0);
      auto curr_right = curr->right();
      rotate_left(curr, curr_right);
      curr_right->balance(0);
      curr->balance(0);
      other->balance(0);
    } else {
      auto other_balance = other->balance();
      rotate_left(curr, other);
      other->balance(-1 + other_balance);
      curr->balance(1 - other_balance);
    }
    assert(curr->check_balance());
    assert(other->check_balance());
  }
}

template<typename T, typename Comparator>
typename AVLTree<T, Comparator>::NodeType *AVLTree<T, Comparator>::rotate_left(NodeType *subroot,
                                                                               NodeType *right) {
  std::unique_ptr<NodeType> subroot_right = subroot->right_move();
  subroot->right(right->left_move());
  std::unique_ptr<NodeType>
      subroot_move = this->move_node_and_replace(subroot, std::move(subroot_right));
  right->left(std::move(subroot_move));
  return right;
}

template<typename T, typename Comparator>
typename AVLTree<T, Comparator>::NodeType *AVLTree<T, Comparator>::rotate_right(NodeType *subroot,
                                                                                NodeType *left) {
  std::unique_ptr<NodeType> subroot_left = subroot->left_move();
  subroot->left(left->right_move());
  std::unique_ptr<NodeType>
      subroot_move = this->move_node_and_replace(subroot, std::move(subroot_left));
  left->right(std::move(subroot_move));
  return left;
}

template<typename T, typename Comparator>
char AVLTree<T, Comparator>::balance(const_iterator position) {
  if (position == this->end())
    return -3;
  return this->current(position)->balance();
}

template<typename T, typename Comparator>
char AVLTree<T, Comparator>::balance(value_type const &value) {
  return balance(this->find(value));
}
}
#endif